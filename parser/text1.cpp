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

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

#include <string>
#include <iostream>
#include <cassert>
#include "parser1.h"

using namespace std;

/*
 * This module contains all the text manipulation routine.
 */

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void buildPath( string & filename, string & dir, string & outname )
{
   size_t len = dir.length();

   if ( len == 0 ) {
      outname = filename;
   }
   else {
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

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

/*
 * This function check that the UTF-8 string is indeed ascii (needed when
 * constructing variables, enums or #defines).
 */
bool isAsciiStr( const char * str, size_t len )
{
   size_t i;
   
   for ( i = 0; i < len ; i++ ) {
      if ( ! isascii(str[i]) ) return false;
   }
   
   return true;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

bool prettify2( const string & inStr, 
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
cerr << size << " " << lineLength << " " << prefix.length() << endl;
cerr << "where1 = " << where << " " << inStr.length() << endl;
   if ( size > inStr.length() - where ) {
      outStr.append( &inStr[where], inStr.length() - where );
      return false;
   }
   
   for ( i = size-1+where; i >= where; --i ) {
cerr << " i = " << i <<endl;
      if ( inStr[i] == 0x20 ) {
         lastWhite = i;
         break;
      }
   }
   
   assert( lastWhite );
   cerr << " lw  " << lastWhite << endl;
   outStr.append( &inStr[where], lastWhite-where+1 );

cerr << "where2 = " << where << endl;
   where = lastWhite+1;
cerr << "where3 = " << where << endl;
cerr << outStr << endl;

   return true;
}
#if 0
   size_t size, i, lines, j, k, lastWhite, start;
//   char newLine[1];

//   newLine[0] = 0x0A;
   size = inStr.length();
   outStr.clear();
   
   lines = size / lineLength + 2; /* To be safe */

   outStr = prefix;
   j = prefix.length();
   for ( i = where, k = 0, start = where, lastWhite = -1; i < size; ++i ) {

      if ( j == lineLength ) {
         outStr.append( &inStr[start], lastWhite-start );
         where = i;
         return true;
         
         j = lineLength - lastWhite + start;

         start = lastWhite + 1;
         lastWhite = -1;
      }

      if ( inStr[i] == 0x20 ) lastWhite = i;
      j++;
   }

   if ( j > 0 ) {
      outStr.append( &inStr[start], size-start+1 );
   }
   
   return false;
}
#endif
/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void prettify( const string & inStr, 
               const string & prefix, 
               string       & outStr, 
               size_t         lineLength )
{
   size_t size, i, lines, j, k, lastWhite, start;
   char newLine[1];

   newLine[0] = 0x0A;
   size = inStr.length();
   outStr.clear();
   
   lines = size / lineLength + 2; /* To be safe */

   outStr = prefix;
   j = prefix.length();
   for ( i = 0, k = 0, start = 0, lastWhite = -1; i < size; ++i ) {

      if ( j == lineLength ) {
         outStr.append( &inStr[start], lastWhite-start );
         outStr += newLine;
         outStr += prefix;

         j = lineLength - lastWhite + start;

         start = lastWhite + 1;
         lastWhite = -1;
      }

      if ( inStr[i] == 0x20 ) lastWhite = i;
      j++;
   }

   if ( j > 0 ) {
      outStr.append( &inStr[start], size-start+1 );
   }
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

xmlChar * replaceChar( xmlChar * inStr, xmlChar old_c, xmlChar * new_c )
{
   int i, j, k, count = 0;
   xmlChar * outStr;
   
   for ( i = 0; i < xmlStrlen(inStr); i++ ) {
      if ( inStr[i] == old_c ) count++;
   }
   outStr = (xmlChar *)calloc( xmlStrlen(inStr)+1+count*(xmlStrlen(new_c)-1), sizeof(xmlChar) );
   if ( outStr == NULL ) {
      fprintf(stderr, "Malloc error\n" );
      exit(1);
   }

   for ( i = 0, j = 0; i < xmlStrlen(inStr); i++ ) {
      if ( inStr[i] == old_c ) {
         for ( k = 0; k < xmlStrlen(new_c); k++, j++ ) {
            outStr[j] = new_c[k];
         }
      }
      else {
         outStr[j] = inStr[i];
         j++;
      }
   }
   
   return outStr;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

/*
 * This function strips unwanted characters from a UTF-8 string.
 * From what I understand, the specs of XML parsing guarantee that everything
 * between the open and close tags is kept (the parser cannot know what
 * is needed and what can be safely removed). So the text in this construct
    <notice>
      Nothing.
    </notice>
 * will be saved as: "\n      Nothing.\n    " which is obviously not what
 * we want. We strip to a one-line string (removing all whitespace at the 
 * beginning and end and stripping all duplicate white spaces in the middle.
 */
void stripText( xmlChar * inStr, string & outStr )
{
   size_t size, start, end, i, j;
   
   outStr.clear();
   end = size = xmlStrlen(inStr);
   start = 0;
   
   /* Replace all unwanted characters by spaces. */
   for ( i = 0; i < size; ++i ) {
      /* Carriage return... just in case */
      if ( inStr[i] == 0x0D ) inStr[i] = 0x20;
      /* New Line */
      if ( inStr[i] == 0x0A ) inStr[i] = 0x20;
      /* Tabs */
      if ( inStr[i] == 0x09 ) inStr[i] = 0x20;
   }

   /* Where does the string really start? */
   for ( i = 0; i < size; ++i ) {
      if ( inStr[i] != 0x20 ) break;
   }
   start = i;
   
   /* And where does it end? */
   for ( i = size-1; i > start; --i ) {
      if ( inStr[i] != 0x20 ) break;
   }
   end = i;
   
   /* Multiple spaces - let's get rid of them */
   outStr += inStr[start];
   for ( i = start+1, j = 1; i <= end; ++i ) {
      if ( inStr[i] == 0x20 && inStr[i-1] == 0x20 ) continue;
      outStr += inStr[i];
      j++;
   }
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

