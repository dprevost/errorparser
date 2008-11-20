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

#include "parser.h"

/*
 * This module contains all the text manipulation routine.
 */

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void buildPath( char * filename, xmlChar * dir, xmlChar * name )
{
   int len;
   
   if ( dir == NULL || xmlStrlen(dir) == 0 ) {
      if ( xmlStrlen(name) > PATH_MAX-1 ) {
         fprintf( stderr, "File name is too long\n" );
         exit(1);
      }
      strcpy( filename, (char*)name );
   }
   else {
      len = xmlStrlen( dir );
      if ( len + xmlStrlen(name) > PATH_MAX-2 ) {
         fprintf( stderr, "File name is too long\n" );
         exit(1);
      }
      strcpy( filename, (char*)dir );
#if defined(WIN32)
      if ( dir[len-1] != '\\' && dir[len-1] != '/' )
         strcat( filename, "\\" );
#else
      if ( dir[len-1] != '/' ) strcat( filename, "/" );
#endif
      strcat( filename, (char*)name );
   }
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

/*
 * This function escape all characters that needs to be escaped from 
 * the input UTF-8 string.
 */
xmlChar * escapeUnescapedQuotes( xmlChar * inStr )
{
   int i, n = 0;
   xmlChar* outStr = NULL;   

   if ( inStr[0] == '"' ) n++;
   if ( inStr[0] == '\'' ) n++;
   
   for ( i = 1; i < xmlStrlen(inStr); i++ ) {
      if ( inStr[i] == '"'  && inStr[i-1] != '\\' ) n++;
      if ( inStr[i] == '\'' && inStr[i-1] != '\\' ) n++;
   }

   outStr = (xmlChar *)calloc( xmlStrlen(inStr) + 1 + n, sizeof(xmlChar) );
   if ( outStr == NULL ) {
      fprintf( stderr, "Malloc error\n" );
      exit(1);
   }

   n = 0;
   if ( inStr[0] == '"' || inStr[0] == '\'' ) {
      outStr[n] = '\\';
      ++n;
   }
   outStr[n] = inStr[0];
   n++;
   
   for ( i = 1; i < xmlStrlen(inStr); i++, n++ ) {
      if ( inStr[i] == '"' && inStr[i-1] != '\\' ) {
         outStr[n] = '\\';
         ++n;
      }
      if ( inStr[i] == '\'' && inStr[i-1] != '\\' ) {
         outStr[n] = '\\';
         ++n;
      }
      outStr[n] = inStr[i];
   }

   return outStr;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

/*
 * This function checks the content of the UTF-8 string for quotes ('"').
 */
int hasUnescapedQuotes( xmlChar * str )
{
   int i;
   
   if ( str[0] == '"' ) return 1;
   if ( str[0] == '\'' ) return 1;
   
   for ( i = 1; i < xmlStrlen(str); i++ ) {
      if ( str[i] == '"'  && str[i-1] != '\\' ) return 1;
      if ( str[i] == '\'' && str[i-1] != '\\' ) return 1;
   }
   return 0;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

/*
 * This function checks the content of the UTF-8 string for escape sequences.
 */
void hasEscapeSequence( errp_common * commonArgs, xmlChar * str )
{
   int i;
   
   if ( commonArgs->allowEscapes ) return;
   
   for ( i = 1; i < xmlStrlen(str)-1; i++ ) {
      if ( str[i] == '\\' ) {
         if ( commonArgs->allowQuotes ) {
            if ( str[i+1] == '"' || str[i+1] == '\'' ) {
               i++;
               continue;
            }
         }
         if ( str[i+1] == '\\' ) {
            i++;
            continue;
         }
         fprintf( stderr, "Esc. sequences are not allowed, string: %s\n", str );
         exit(1);
      }
   }
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

/*
 * This function check that the UTF-8 string is indeed ascii (needed when
 * constructing variables, enums or #defines).
 */
int isAsciiStr( xmlChar * str )
{
   int i;
   
   for ( i = 0; i < xmlStrlen(str); i++ ) {
      if ( str[i] > 0x7f ) return 0;
   }
   return 1;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

xmlChar * prettify( xmlChar* inStr, char* prefix, int lineLength )
{
   xmlChar* outStr = NULL;   
   int size, i, lines, j, k, lastWhite, start;
   char newLine[1];

   newLine[0] = 0x0A;
   size = xmlStrlen(inStr);
   
   lines = size / lineLength + 2; /* To be safe */
   outStr = (xmlChar *)calloc( size+lines*(strlen(prefix)+1), sizeof(xmlChar) );
   if ( outStr == NULL ) {
      fprintf(stderr, "Malloc error\n" );
      exit(1);
   }

   strcat( (char*)outStr, prefix );
   j = strlen(prefix);
   for ( i = 0, k = 0, start = 0, lastWhite = -1; i < size; ++i ) {

      if ( j == lineLength ) {
         strncat( (char*)outStr, (char*)&inStr[start], lastWhite-start );
         strncat( (char*)outStr, newLine, 1 );
         strcat( (char*)outStr, prefix );

         j = lineLength - lastWhite + start;

         start = lastWhite + 1;
         lastWhite = -1;
      }

      if ( inStr[i] == 0x20 ) lastWhite = i;
      j++;

      
   }

   if ( j > 0 ) {
      strncat( (char*)outStr, (char*)&inStr[start], size-start+1 );
   }
   return outStr;
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
xmlChar * stripText( xmlChar * inStr )
{
   xmlChar* outStr = NULL;
   
   int size, start, end, i, j;
   
   end = size = xmlStrlen(inStr);
   start = 0;
   
   outStr = (xmlChar *)calloc( (size+1), sizeof(xmlChar) );
   if ( outStr == NULL ) {
      fprintf( stderr, "Malloc error\n" );
      exit(1);
   }

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
   outStr[0] = inStr[start];
   for ( i = start+1, j = 1; i <= end; ++i ) {
      if ( inStr[i] == 0x20 && inStr[i-1] == 0x20 ) continue;
      outStr[j] = inStr[i];
      j++;
   }
   
   return outStr;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

