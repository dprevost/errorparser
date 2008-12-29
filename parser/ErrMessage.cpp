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

#include <iostream>
#include "ErrMessage.h"
#include "parser.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

ErrMessage::ErrMessage( string & filename,
                        string & header,
                        string & prefix,
                        string & varPrefix,
                        bool     allowEscapes,
                        bool     allowQuotes )
   : errorCount ( 0 ),
     headerName ( header ),
     prefix     ( prefix ),
     varPrefix  ( varPrefix ),
     allowEscapes ( allowEscapes ),
     allowQuotes  ( allowQuotes )
{
   outStream.open( filename.c_str(), fstream::out );
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrMessage::addTopCode()
{
   // Need to include a system header file to get the definition of NULL.
   outStream << "#include <stdlib.h> /* Needed for NULL. */" << endl << endl;

   // This define is use if the output is part of the library and if
   // the Win32 decorations (import/export) are needed.
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

void ErrMessage::addError( const std::string & errNumber,
                           const std::string & errName,
                           xmlNode           * messageNode )
{
   xmlNode * node;
   string errMessage, tmp;
   
   outStream << "/* " << prefix << "_" << errName << " */" << endl;
   outStream << varPrefix << "_MsgStruct " << varPrefix << "_Msg" <<
      errorCount << " = {" << endl;
   outStream << "    " << errNumber << ", ";
   
   node = messageNode->children;
   
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }

   stripText( node->children->content, errMessage );

   /* Check for escape sequences */
   hasEscapeSequence( errMessage );

   if ( hasUnescapedQuotes(errMessage) ) {
      if ( ! allowQuotes ) {
         cerr << "Quotes are not allowed, string: " << errMessage << endl;
         exit(1);
      }
      escapeUnescapedQuotes( errMessage, tmp );
      outStream << "\"" << tmp << "\" };" << endl <<endl;
   }
   else {
      outStream << "\"" << errMessage << "\" };" << endl << endl;
   }

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
      outStream << "    &" << varPrefix << "_Msg" << i << endl;
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

void ErrMessage::hasEscapeSequence( string & str )
{
   size_t i;
   
   if ( allowEscapes ) return;
   
   for ( i = 1; i < str.length()-1; i++ ) {
      if ( str[i] == '\\' ) {
         if ( allowQuotes ) {
            if ( str[i+1] == '"' || str[i+1] == '\'' ) {
               i++;
               continue;
            }
         }
         if ( str[i+1] == '\\' ) {
            i++;
            continue;
         }
         cerr << "Esc. sequences are not allowed, string: " << str << endl;
         exit(1);
      }
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

// This function checks the content of the UTF-8 string for quotes ('"').
bool ErrMessage::hasUnescapedQuotes( string & str )
{
   size_t i;
   
   if ( str[0] == '"' ) return true;
   if ( str[0] == '\'' ) return true;
   
   for ( i = 1; i < str.length(); i++ ) {
      if ( str[i] == '"'  && str[i-1] != '\\' ) return true;
      if ( str[i] == '\'' && str[i-1] != '\\' ) return true;
   }
   
   return false;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

// This function escape all characters that needs to be escaped from 
// the input UTF-8 string.

void ErrMessage::escapeUnescapedQuotes( const string & inStr, string & outStr )
{
   size_t i;

   outStr.clear();
   
   if ( inStr[0] == '"' || inStr[0] == '\'' ) {
      outStr += '\\';
   }
   outStr += inStr[0];
   
   for ( i = 1; i < inStr.length(); i++ ) {
      if ( inStr[i] == '"' && inStr[i-1] != '\\' ) {
         outStr += '\\';
      }
      if ( inStr[i] == '\'' && inStr[i-1] != '\\' ) {
         outStr += '\\';
      }
      outStr += inStr[i];
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

