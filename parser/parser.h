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

#ifndef ERRP_PARSER_H
#define ERRP_PARSER_H

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#if defined WIN32
#  include "config-win32.h"
   /* 
    * The pragma is to hide a warning in Microsoft include files
    * with VC++ 6. Don't know about other versions yet.
    */
#  pragma warning(disable:4115)
#  include <windows.h>
#  pragma warning(default:4115)
#  include <winbase.h>
#  include <io.h>
#  include <process.h> 
#endif

#include <stdio.h>
#if HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif
#if STDC_HEADERS
#  include <stdlib.h>
#  include <stddef.h>
#  include <stdarg.h>
#else
#  if HAVE_STDLIB_H
#    include <stdlib.h>
#  endif
#endif
#if HAVE_STRING_H
#  if !STDC_HEADERS && HAVE_MEMORY_H
#    include <memory.h>
#  endif
#  include <string.h>
#endif
#if HAVE_STRINGS_H
#  include <strings.h>
#endif

#include <time.h>

#if HAVE_CTYPE_H
#  include <ctype.h>
#endif

#ifndef PATH_MAX
#  if defined MAXPATHLEN 
#    define PATH_MAX MAXPATHLEN 
#  elif defined _POSIX_PATH_MAX
#    define PATH_MAX _POSIX_PATH_MAX
#  elif defined _MAX_PATH
#    define PATH_MAX  _MAX_PATH
#  else
#    define PATH_MAX 1024
# endif
#endif

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

#define ERRP_LINE_LENGTH 72

/* 
 * A "common" struct to pass all the parameters around (instead of
 * using many globals or passing tons of parameters to the different 
 * functions...
 */
struct errp_common {
   xmlChar * xmlFileName;
   xmlDoc  * document;
   FILE    * fpHeader;
   FILE    * fpMsgC;
   FILE    * fpMsgH;
   xmlChar * headerName;
   xmlChar * outputNameC;
   xmlChar * outputNameH;
   xmlChar * enumname;
   char      outputNameGuard[PATH_MAX];
   xmlChar * varPrefix;
   int       writingEnum;
   xmlChar * prefix;
   int       errorCount;
   int       groupCount;
   int       allowEscapes;
   int       allowQuotes;
   xmlChar * percent;
};

typedef struct errp_common errp_common;

extern const char * g_barrier;
extern const char * g_functionName;

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

/* Prototypes of text.c functions */

xmlChar * escapeUnescapedQuotes( xmlChar * inStr );
int hasUnescapedQuotes( xmlChar * str );
void hasEscapeSequence( errp_common * commonArgs, xmlChar * str );
int isAsciiStr( xmlChar * str );
xmlChar * prettify( xmlChar* inStr, char* prefix, int lineLength );
xmlChar * replaceChar( xmlChar * inStr, xmlChar old_c, xmlChar * new_c );
xmlChar * stripText( xmlChar * inStr );

void writeErrorMessage( errp_common * commonArgs );

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

#endif /* ERRP_PARSER_H */
