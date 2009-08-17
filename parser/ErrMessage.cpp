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

#include "ErrMessage.h"
#include "parser.h"
#include "MessageXML.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

ErrMessage::ErrMessage( string & filename,
                        string & header,
                        string & inPrefix,
                        string & varPrefix )
   : errorCount ( 0 ),
     headerName ( header ),
     prefix     ( inPrefix ),
     varPrefix  ( varPrefix )
{
   outStream.exceptions( fstream::failbit | fstream::badbit);
   outStream.open( filename.c_str(), fstream::out );

   if ( prefix.length() > 0 ) prefix += '_';
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrMessage::addTopCode()
{
   // Need to include a system header file to get the definition of NULL.
   outStream << "#include <stdlib.h> /* Needed for NULL. */" << endl << endl;

   // This define is use if the output is part of the library and if
   // the Win32 decorations (import/export) are needed. Otherwise
   // it is useless but we write it anyway (cost nothing).
   outStream << "#define BULDING_ERROR_MESSAGE" << endl;
   outStream << "#include \"" << headerName << "\"" << endl << endl;
   
   // The definition of the basic struct for holding error information
   outStream << "struct " << varPrefix << "_MsgStruct" << endl;
   outStream << "{" << endl;
   outStream << "    int  errorNumber;" << endl;
   outStream << "    const char * message;" << endl;
   outStream << "};" << endl << endl;
   outStream << "typedef struct " << varPrefix << "_MsgStruct " << 
      varPrefix << "_MsgStruct;" << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrMessage::addError( ErrorXML & error, bool lastError )
{
   string errMessage;
   string & errNumber = error.GetErrNumber();
   string & errName   = error.GetErrName();
   const char * msg;
   
   // Maybe we should write the documentation instead of just the
   // error name?
   outStream << "/* " << prefix << errName << " */" << endl;

   outStream << varPrefix << "_MsgStruct " << varPrefix << "_Msg" <<
      errorCount << " = {" << endl;
   outStream << "    " << errNumber << ", ";
   
   msg = error.GetErrMessage();
   g_msgXML->GetErrMessage( msg, errMessage );
   
   outStream << "\"" << errMessage << "\" };" << endl << endl;
   
   errorCount++;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrMessage::addBottomCode()
{
   int i;
   
   outStream << barrier << endl << endl;
   
   // Define an array of pointers to the previous structs.
   outStream << "/* Array of pointers to previous structs */" << endl;
   outStream << varPrefix << "_MsgStruct * " << varPrefix << 
      "_MsgArray[" << errorCount << "] = {" << endl;

   for ( i = 0; i < errorCount-1; ++i ) {
      outStream << "    &" << varPrefix << "_Msg" << i << "," << endl;
   }
   outStream << "    &" << varPrefix << "_Msg" << errorCount-1 << endl;
   outStream << "};" << endl << endl;

   outStream << barrier << endl << endl;

   // Write the function to access the error messages
   outStream << "const char * " << varPrefix << "_ErrorMessage( int errnum )" << endl;
   outStream << "{" << endl;
   outStream << "    int i;" << endl << endl;

   outStream << "    for ( i = 0; i < " << errorCount << "; ++i ) {" << endl;
   outStream << "        if ( errnum == " << varPrefix << "_MsgArray[i]->errorNumber ) {" << endl;
   outStream << "            return " << varPrefix << "_MsgArray[i]->message;" << endl;
   outStream << "        }" << endl;
   outStream << "    }" << endl << endl;

   outStream << "    return NULL;" << endl;
   outStream << "}" << endl << endl;

   outStream << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

