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
   
   barrier = "// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--";
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
   bool firstpara = true;
   string tmp;
   
   node = messageNode->children;
   
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   
   // jump over the error message - we only want the docu itself
   node = node->next;
   
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         /* This can only be a paragraph of the documentation */
         stripText( node->children->content, tmp );
         
         if ( firstpara ) firstpara = false;
         else {
            out_stream << indent << "    //" << endl;
         }
         if ( my_namespace.length() == 0 ) {
            prettify( out_stream, tmp, "    // ", ERRP_LINE_LENGTH );
         }
         else {
            prettify( out_stream, tmp, "        // ", ERRP_LINE_LENGTH );
         }
      }
      node = node->next;
   }

   out_stream << indent << "    " << errName << " = " << errNumber;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void CSharp::addErrorTrailer()
{
   out_stream << "," << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void CSharp::addBottomCode()
{
   // Added at the end of the last error
   out_stream << endl;
   
   out_stream << indent << "}" << endl;
   if ( my_namespace.length() > 0 ) {
      out_stream << "}" << endl;
   }
   out_stream << endl << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

