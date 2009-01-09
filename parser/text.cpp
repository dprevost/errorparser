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

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>

#include <string.h> // strlen

#include "parser.h"

using namespace std;

#if !defined(PATH_MAX)
#  define PATH_MAX _MAX_PATH
#endif

// This module contains utilities used by other objects of this program.

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void buildPath( string & dir, string & filename, string & outname )
{
   size_t len = dir.length();
 
   if ( len == 0 ) {
      outname = filename;
   }
   else {
      outname = dir;
#if defined(WIN32)
      if ( dir[len-1] != '\\' && dir[len-1] != '/' ) outname += "\\";
#else
      if ( dir[len-1] != '/' ) outname += "/";
#endif
      outname += filename;      
   }
   
   if ( outname.length() >= PATH_MAX ) {
      cerr << "File name is too long!" << endl;
      exit(1);
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

// This function check that the input string is indeed ascii (needed when
// constructing variables, enums or #defines).
bool isAsciiStr( const char * str, size_t len )
{
   size_t i;
   
   for ( i = 0; i < len ; i++ ) {
      if ( ! isascii(str[i]) ) return false;
   }
   
   return true;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

// Function internal to this module
bool doFormat( const string & inStr, 
               size_t       & where,
               const string & prefix,
               string       & outStr,
               size_t         lineLength )
{
   size_t size, i, lastWhite = 0;
   
   outStr.clear();
   outStr = prefix;
   
   assert( where < inStr.length() );
   
   size = lineLength - prefix.length();
   if ( size > inStr.length() - where ) {
      outStr.append( &inStr[where], inStr.length() - where );
      return false;
   }
   
   for ( i = size-1+where; i >= where; --i ) {
      if ( inStr[i] == 0x20 ) {
         lastWhite = i;
         break;
      }
   }
   
   if ( lastWhite == 0 ) {
      // Technically possible (if prefix is quite large, for example)
      // but not really expected...
      lastWhite = size-1+where;
   }
   
   outStr.append( &inStr[where], lastWhite-where+1 );

   where = lastWhite+1;

   return true;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

// Format text will format the input string into lines with a maximum
// length of lineLength - each line will start with "prefix" (the length
// of prefix is counted in the length of a line).
// 
// Special note 1: this function will attempt to break lines on natural
//                 borders (space between words). 
// Special note 2: the input string should not contain formatting information
//                 (like \n). Use stripText() to sanitize it as needed.
void formatText( fstream      & outStream,
                 const string & inStr, 
                 const string & prefix, 
                 size_t         lineLength )
{
   size_t where = 0;
   string line;
   
   while ( doFormat(inStr, where, prefix, line, lineLength) ) {
      outStream << line << endl;
   }
   outStream << line << endl;

}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/*
 * This function strips unwanted characters from a UTF-8 string.
 * From what I understand, the specs of XML parsing guarantee that everything
 * between the open and close tags is kept (the parser cannot know what
 * is needed and what can be safely removed). So the text in this construct
    <notice>
      Nothing.
    </notice>
 * will be saved as: "\n      Nothing.\n    " which is obviously not what
 * we want. We strip to a one-line string (removing all whitespace and 
 * newlines at the beginning and end and stripping all duplicate white spaces 
 * in the middle).
 */
void stripText( const char * inStr, string & outStr )
{
   size_t size, start, end, i;
   bool previousWasSpace = false;

   outStr.clear();
   size = strlen(inStr);
   start = size;
   end = 0;
   
   // Where does the string really start?
   for ( i = 0; i < size; ++i ) {
      // Written for clarity. Letting the compiler optimize it...
      
      // Carriage return
      if ( inStr[i] == 0x0D ) continue;
      // New line
      if ( inStr[i] == 0x0A ) continue;
      // Tab and space
      if ( isspace( inStr[i] ) ) continue;
         
      start = i;
      break;
   }
   
   // And where does it end?
   for ( i = size-1; i >= start; --i ) {
      // Written for clarity. Letting the compiler optimize it...
      
      // Carriage return
      if ( inStr[i] == 0x0D ) continue;
      // New line
      if ( inStr[i] == 0x0A ) continue;
      // Tab and space
      if ( isspace( inStr[i] ) ) continue;

      end = i;
      break;
   }
   
   // Process the string
   for ( i = start; i <= end; ++i ) {
      if ( inStr[i] == 0x0D || inStr[i] == 0x0A || isspace(inStr[i]) ) {
         if ( previousWasSpace ) continue;
         else {
            outStr += ' ';
            previousWasSpace = true;
         }
      }
      else {
         outStr += inStr[i];
         previousWasSpace = false;
      }
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

