/*
 * Copyright (C) 2009 Daniel Prevost <dprevost@users.sourceforge.net>
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

#include "Java.h"
#include "parser.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

Java::Java( string & filename, string & java_package, string & java_enum )
   : my_package( java_package ),
     my_enum   ( java_enum )
{
   outStream.open( filename.c_str(), fstream::out );
   
   barrier = "// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--";
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void Java::addTopCode()
{
   if ( my_package.length() > 0 ) {
      outStream << "package " << my_package << ";" << endl << endl;
   }
   
   outStream << "public enum " << my_enum << " {" << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void Java::addError( ErrorXML & error, bool lastError )
{
   bool firstpara = true;
   string tmp;
   string & errNumber = error.GetErrNumber();
   string & errName   = error.GetErrName();
   const char * paragraph;
   string errMessage;
   const char * msg;
   
   paragraph = error.GetDocuParagraph();
   while ( paragraph != NULL ) {
      stripText( paragraph, tmp );
         
      if ( firstpara ) firstpara = false;
      else {
         outStream << "    //" << endl;
      }
      formatText( outStream, tmp, "    // ", ERRP_LINE_LENGTH );

      paragraph = error.GetDocuParagraph();
   }

   msg = error.GetErrMessage();
   stripText( msg, errMessage );

   outStream << "    " << errName << "(" << errNumber << ") {" << endl;
   outStream << "        String getMessage() { return \"" << errMessage << "\"; } }";

   if (lastError) outStream << ";" << endl << endl;
   else outStream << "," << endl << endl;
   
   //   const char * error.GetErrMessage();

}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void Java::addBottomCode()
{
   outStream << "    " << barrier << endl << endl;
   outStream << "    private final int errcode;" << endl << endl;
   outStream << "    " << my_enum << "(int errcode) { " << endl;
   outStream << "        this.errcode = errcode;" << endl;
   outStream << "    }" << endl << endl;
   outStream << "    public int getErrorNumber() { return errcode; }" << endl << endl;
   
   outStream << "    abstract String getMessage();" << endl << endl;
           
   outStream << "}" << endl;

   outStream << endl << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

