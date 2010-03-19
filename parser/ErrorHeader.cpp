/* -*- c++ -*- */
/* :mode=c++:  - For jedit, previous line for emacs */
/*
 * Copyright (C) 2008-2009 Daniel Prevost <dprevost@users.sourceforge.net>
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

// Microsoft specific. Must be defined before including system header
// files (this will avoid a warning if we ever use a C library function 
// declared by Microsoft as deprecated.
#define _CRT_SECURE_NO_DEPRECATE

#include <string>
#include <fstream>
#include <iostream>

#include "ErrorHeader.h"
#include "parser.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

ErrorHeader::ErrorHeader( string & dir,
                          string & header,
                          string & ename,
                          string & inPrefix )
   : HeaderHandler( header ),
     enumName     ( ename ),
     prefix       ( inPrefix ),
     usingEnum    ( false )
{
   string name;
   
   buildPath( dir, header, name );
   
   outStream.exceptions( fstream::failbit | fstream::badbit);
   outStream.open( name.c_str(), fstream::out );

   if ( enumName.length() > 0 ) usingEnum = true;
   
   if ( prefix.length() > 0 ) prefix += '_';
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrorHeader::addTopCode()
{
   // enum information is only present if the target is an enum. If not
   // present, we use "#define" instead.
   if ( usingEnum ) {
      outStream << "enum " << enumName << endl;
      outStream << "{" << endl;
   }

}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrorHeader::addGroupIdent( GroupIdent & ident ) throw( MissingException )
{
   const char * paragraph;
   string tmp;
   
   // We call formatText since we have no control over the length
   // of that field in the xml input file.
   if ( usingEnum ) {
      outStream << "    /*" << endl;
      formatText( outStream, ident.GetName(), "     * ", ERRP_LINE_LENGTH );
   }
   else {
      outStream << "/*" << endl;
      formatText( outStream, ident.GetName(), " * ", ERRP_LINE_LENGTH );
   }

   paragraph = ident.GetDescParagraph();
   while ( paragraph != NULL ) {
      stripText( paragraph, tmp );
      if ( tmp.empty() ) throw new MissingException( "<errgroup_desc>" );

      if ( usingEnum ) {
         outStream << "     *" << endl;
         formatText( outStream, tmp, "     * ", ERRP_LINE_LENGTH );
      }
      else {
         outStream << " *" << endl;
         formatText( outStream, tmp, " * ", ERRP_LINE_LENGTH );
      }
      paragraph = ident.GetDescParagraph();
   }

   if ( usingEnum ) {
      outStream << "     */" << endl << endl;
   }
   else {
      outStream << " */" << endl << endl;
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrorHeader::addError( ErrorXML & error,
                            bool       lastError ) throw( MissingException )
{
   bool firstpara = true;
   string tmp;
   string & errNumber = error.GetErrNumber();
   string & errName   = error.GetErrName();
   const char * paragraph;

   // We start by writing the documentation in a format understood by
   // doxygen.
   if ( usingEnum ) {
      outStream << "    /**" << endl;
   }
   else {
      outStream << "/**" << endl;
   }

   paragraph = error.GetDocuParagraph();
   while ( paragraph != NULL ) {
      stripText( paragraph, tmp );
      if ( tmp.empty() ) throw new MissingException( "<errordoc>" );
         
      if ( firstpara ) firstpara = false;
      else {
         if ( usingEnum ) {
            outStream << "     *" << endl;
         }
         else {
            outStream << " *" << endl;
         }
      }

      if ( usingEnum ) {
         formatText( outStream, tmp, "     * ", ERRP_LINE_LENGTH );
      }
      else {
         formatText( outStream, tmp, " * ", ERRP_LINE_LENGTH );
      }

      paragraph = error.GetDocuParagraph();
   }

   if ( usingEnum ) {
      outStream << "     */" << endl;
   }
   else {
      outStream << " */" << endl;
   }

   // We write the errcodes (name and value)
   if ( usingEnum ) {
      outStream << "    " << prefix << errName << " = " << errNumber;
   }
   else {
      outStream << "#define " << prefix << errName << " " << errNumber;
   }
   
   if ( ! lastError ) {
      if ( usingEnum ) {
         outStream << ",";
      }
      outStream << endl << endl;
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrorHeader::addBottomCode()
{
   if ( usingEnum ) {
      outStream << endl;
      outStream << "};" << endl << endl;
      outStream << "typedef enum " << enumName << " " << enumName << ";" << endl << endl;
      outStream << barrier << endl << endl;
   }
   else {
      outStream << endl << endl;
      outStream << barrier << endl << endl;
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

