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

#include "ErrMessage.h"
#include "parser1.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

ErrMessage::ErrMessage( string & dirname,
                        string & filename,
                        string & header,
                        string & prefix )
   : errorCount ( 0 ),
     headerName ( header ),
     varPrefix  ( prefix )
{
   string name;
   
   buildPath( dirname, filename, name );
   
   out_stream.open( name.c_str(), fstream::out );
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrMessage::addTopCode()
{
   // Need to include a system header file to get the definition of NULL.
   out_stream << "#include <stdlib.h> /* Needed for NULL. */" << endl << endl;

   // This define is use if the output is part of the library and if
   // the Win32 decorations (import/export) are needed.
   out_stream << "#define BULDING_ERROR_MESSAGE" << endl;
   out_stream << "#include \"" << headerName << "\"" << endl << endl;
   
   // The definition of the basic struct for holding error information
   out_stream << "struct " << varPrefix << "_MsgStruct" << endl;
   out_stream << "{" << endl;
   out_stream << "    int  errorNumber;" << endl;
   out_stream << "    const char * message;" << endl;
   out_stream << "};" << endl << endl;
   out_stream << "typedef struct " << varPrefix << "_MsgStruct " << 
      varPrefix << "_MsgStruct;" << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrMessage::addBottomCode()
{
   int i;
   
   out_stream << barrier << endl << endl;
   
   // Define an array of pointers to the previous structs.
   out_stream << "/* Array of pointers to previous structs */" << endl;
   out_stream << varPrefix << "_MsgStruct * " << varPrefix << 
      "_MsgArray[" << errorCount << "] = {" << endl;

   for ( i = 0; i < errorCount-1; ++i ) {
      out_stream << "    &" << varPrefix << "_Msg" << i << endl;
   }
   out_stream << "    &" << varPrefix << "_Msg" << errorCount-1 << endl;
   out_stream << "};" << endl << endl;

   out_stream << barrier << endl << endl;

   // Write the function to access the error messages
   out_stream << "const char * " << varPrefix << "_ErrorMessage( int errnum )" << endl;
   out_stream << "{" << endl;
   out_stream << "    int i;" << endl << endl;

   out_stream << "    for ( i = 0; i < " << errorCount << "; ++i ) {" << endl;
   out_stream << "        if ( errnum == " << varPrefix << "_MsgArray[i]->errorNumber ) {" << endl;
   out_stream << "            return " << varPrefix << "_MsgArray[i]->message;" << endl;
   out_stream << "        }" << endl;
   out_stream << "    }" << endl << endl;

   out_stream << "    return NULL;" << endl;
   out_stream << "}" << endl << endl;

   out_stream << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
