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

#include "MessageXML.h"
#include "parser.h"

using namespace std;

MessageXML * g_msgXML = NULL;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

MessageXML::MessageXML( bool     allowEscapes,
                        bool     allowQuotes,
                        string & percent )
   : allowEscapes( allowEscapes ),
     allowQuotes ( allowQuotes ),
     percent     ( percent )
{
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void MessageXML::GetErrMessage( const char * nodeMsg,
                                string     & message ) throw( MissingException )
{
   string errMessage, tmp;
   
   stripText( nodeMsg, tmp );
   if ( tmp.empty() ) throw new MissingException( "<errormsg>" );
   stripPercent( tmp, errMessage );
   tmp.clear();
   
   /* Check for escape sequences */
   hasEscapeSequence( errMessage );

   if ( hasUnescapedQuotes(errMessage) ) {
      if ( ! allowQuotes ) {
         cerr << "Quotes are not allowed, string: " << errMessage << endl;
         exit(1);
      }
      escapeUnescapedQuotes( errMessage, tmp );
      message = tmp;
   }
   else {
      message = errMessage;
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void MessageXML::hasEscapeSequence( string & str )
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

// This function checks the content of the string for single and double
// quotes (" and '). These two needs to be escaped (but not `).
bool MessageXML::hasUnescapedQuotes( string & str )
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

// This function escape all quotes that need to be escaped from 
// the input string.

void MessageXML::escapeUnescapedQuotes( const string & inStr, string & outStr )
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

void MessageXML::stripPercent( const std::string & inStr, 
                               std::string       & outStr )
{
   size_t i;

   outStr.clear();
   
   for ( i = 0; i < inStr.length(); i++ ) {
      if ( inStr[i] == '%' ) {
         outStr += percent;
      }
      else outStr += inStr[i];
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
