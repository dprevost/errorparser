/* -*- c++ -*- */
/* :mode=c++:  - For jedit, previous line for emacs */
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

#include "ErrorHeader.h"
#include "parser1.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

ErrorHeader::ErrorHeader( string & dir,
                          string & header,
                          string & ename,
                          string & prefix )
   : HeaderHandler( header ),
     enumName     ( ename ),
     prefix       ( prefix ),
     usingEnum    ( false )
{
   string name;
   
   buildPath( dir, header, name );
   
   out_stream.open( name.c_str(), fstream::out );

   if ( enumName.length() > 0 ) usingEnum = true;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrorHeader::addTopCode()
{
   /* 
    * enum information is only present if the target is an enum. If not
    * present, we use "#define" instead.
    */
   if ( usingEnum ) {
      out_stream << "enum " << enumName << endl;
      out_stream << "{" << endl;
   }

}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrorHeader::addGroupName( std::string name )
{
   string prettyName;
   
   // We call prettify since we have no control over the length
   // of that field in the xml input file.
   if ( usingEnum ) {
      out_stream << "    /*" << endl;
      prettify( name, "     * ", prettyName, ERRP_LINE_LENGTH );
   }
   else {
      out_stream << "/*" << endl;
      prettify( name, " * ", prettyName, ERRP_LINE_LENGTH );
   }

   out_stream << prettyName << endl;
}
   
// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrorHeader::addGroupDesc( std::string description )
{
   string prettyDesc;
   
   if ( usingEnum ) {
      out_stream << "     *" << endl;
      prettify( description, "     * ", prettyDesc, ERRP_LINE_LENGTH );
   }
   else {
      out_stream << " *" << endl;
      prettify( description, " * ", prettyDesc, ERRP_LINE_LENGTH );
   }
   
   out_stream << prettyDesc << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrorHeader::endGroupDesc()
{
   if ( usingEnum ) {
      out_stream << "     */" << endl << endl;
   }
   else {
      out_stream << " */" << endl << endl;
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrorHeader::addError( const std::string & errNumber,
                            const std::string & errName,
                            xmlNode           * messageNode )
{
   xmlNode * node;
   int firstpara = 1;
   string tmp, paragraph;

   node = messageNode->children;
   
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   
   // jump over the error message - we only want the docu itself
   node = node->next;

   if ( usingEnum ) {
      out_stream << "    /**" << endl;
   }
   else {
      out_stream << "/**" << endl;
   }

   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         /* This can only be a paragraph of the documentation */
         stripText( node->children->content, tmp );
         
         if ( firstpara ) firstpara = 0;
         else {
            if ( usingEnum ) {
               out_stream << "     *" << endl;
            }
            else {
               out_stream << " *" << endl;
            }
         }

         if ( usingEnum ) {
            prettify( tmp, "     * ", paragraph, ERRP_LINE_LENGTH );
         }
         else {
            prettify( tmp, " * ", paragraph, ERRP_LINE_LENGTH );
         }
         out_stream << paragraph << endl;
      }
      node = node->next;
   }

   if ( usingEnum ) {
      out_stream << "     */" << endl;
      out_stream << "    " << prefix << "_" << errName << " = " << errNumber;
   }
   else {
      out_stream << " */" << endl;
      out_stream << "#define " << prefix << "_" << errName << " " << errNumber;
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrorHeader::addBottomCode()
{
   if ( usingEnum ) {
      out_stream << "};" << endl << endl;
      out_stream << "typedef enum " << enumName << " " << enumName << ";" << endl << endl;
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

