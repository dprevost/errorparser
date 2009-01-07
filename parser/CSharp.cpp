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

#include "CSharp.h"
#include "parser.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

CSharp::CSharp( string & filename, string & cs_namespace, string & cs_enum )
   : my_namespace( cs_namespace ),
     my_enum     ( cs_enum ),
     indent      ( "" )
{
   outStream.open( filename.c_str(), fstream::out );
   
   if ( my_namespace.length() > 0 ) indent = "    ";
   
   barrier = "// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--";
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void CSharp::addTopCode()
{
   outStream << "using System;" << endl << endl;

   if ( my_namespace.length() > 0 ) {
      outStream << "namespace " << my_namespace << endl << "{" << endl;
   }
   
   outStream << indent << "public enum " << my_enum << endl;
   outStream << indent << "{" << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void CSharp::addError( ErrorXML & error, bool lastError )
{
   bool firstpara = true;
   string tmp;
   string & errNumber = error.GetErrNumber();
   string & errName   = error.GetErrName();
   xmlChar * paragraph;
   
   paragraph = error.GetDocuParagraph();
   while ( paragraph != NULL ) {
      stripText( paragraph, tmp );
         
      if ( firstpara ) firstpara = false;
      else {
         outStream << indent << "    //" << endl;
      }
      if ( my_namespace.length() == 0 ) {
         prettify( outStream, tmp, "    // ", ERRP_LINE_LENGTH );
      }
      else {
         prettify( outStream, tmp, "        // ", ERRP_LINE_LENGTH );
      }

      paragraph = error.GetDocuParagraph();
   }

   outStream << indent << "    " << errName << " = " << errNumber;
   if (! lastError) outStream << "," << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void CSharp::addBottomCode()
{
   // Added at the end of the last error
   outStream << endl;
   
   outStream << indent << "}" << endl;
   if ( my_namespace.length() > 0 ) {
      outStream << "}" << endl;
   }
   outStream << endl << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

