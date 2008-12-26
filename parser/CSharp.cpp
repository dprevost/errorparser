/*
 * Copyright (C) 2008 Daniel Prevost <dprevost@users.sourceforge.net>
 *
 * This file may be distributed and/or modified under the terms of the
 * MIT License as described by the Open Source Initiative
 * (http://opensource.org/licenses/mit-license.php) and appearing in
 * the file COPYING included in the packaging of this software.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#include "CSharp.h"
#include "parser1.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

CSharp::CSharp( string & filename, string & cs_namespace, string & cs_enum )
   : my_namespace( cs_namespace ),
     my_enum     ( cs_enum ),
     indent      ( "" )
{
   out_stream.open( filename.c_str(), fstream::out );
   
   if ( my_namespace.length() > 0 ) indent = "    ";
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void CSharp::addTopCode()
{
   out_stream << "using System;" << endl << endl;

   if ( my_namespace.length() > 0 ) {
      out_stream << "namespace " << my_namespace << endl << "{" << endl;
   }
   
   out_stream << indent << "public enum " << my_enum << endl;
   out_stream << indent << "{" << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void CSharp::addError( const string & errNumber, 
                       const string & errName, 
                       xmlNode      * messageNode )
{
   xmlNode * node;
   int firstpara = 1;
   string tmp, paragraph;
   
   node = messageNode->children;
   
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   
   // jump over the error message - we only want the docu itself
   node = node->next;
   
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         /* This can only be a paragraph of the documentation */
         stripText( node->children->content, tmp );
         
         if ( firstpara ) firstpara = 0;
         else {
            out_stream << indent << "    //" << endl;
         }
         if ( my_namespace.length() == 0 ) {
            prettify( tmp, "    // ", paragraph, ERRP_LINE_LENGTH );
         }
         else {
            prettify( tmp, "        // ", paragraph, ERRP_LINE_LENGTH );
         }
         out_stream << paragraph << endl;
      }
      node = node->next;
   }

   out_stream << indent << "    " << errName << " = " << errNumber;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void CSharp::addBottomCode()
{
   out_stream << indent << "}" << endl;
   if ( my_namespace.length() > 0 ) {
      out_stream << "}" << endl;
   }
   out_stream << endl << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
#if 0
void addMessages( errp_common * commonArgs,
                  xmlNode     * messages )
{
   xmlNode * node = NULL;
   xmlChar * errMessage, * tmp;
   
   node = messages->children;
   
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   errMessage = stripText( node->children->content );
   
   // jump over the error message - we only want the docu itself
   node = node->next;
   
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         addDocumentationCS( commonArgs, node );
         break;
      }
      node = node->next;
   }

}
void addDocumentationCS( errp_common * commonArgs,
                         xmlNode     * node )
{
   xmlChar * paragraph, * tmp1;
   int firstpara = 1;
   
   do {
      if ( node->type == XML_ELEMENT_NODE ) {
         /* This can only be a paragraph of the documentation */
         tmp1 = stripText( node->children->content );
         
         if ( firstpara ) firstpara = 0;
         else {
            if ( commonArgs->cs_namespace == NULL ) {
               fprintf( commonArgs->fpCS, "    //\n" );
            }
            else {
               fprintf( commonArgs->fpCS, "        //\n" );
            }
         }
         if ( commonArgs->cs_namespace == NULL ) {
            paragraph = prettify( tmp1, "    // ", ERRP_LINE_LENGTH );
         }
         else {
            paragraph = prettify( tmp1, "        // ", ERRP_LINE_LENGTH );
         }
         fprintf( commonArgs->fpCS, "%s\n", paragraph );

         free( paragraph );
         free( tmp1 );
      }
      
      node = node->next;

   } while ( node != NULL );
}


#endif
