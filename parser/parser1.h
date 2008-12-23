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
#  define _CRT_SECURE_NO_DEPRECATE
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

#include <string>

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
   std::string xmlFileName;
   xmlDoc  * document;
   FILE    * fpHeader;
   FILE    * fpMsgC;
   FILE    * fpMsgH;
   FILE    * fpCS;
   FILE    * fpPyH;
   FILE    * fpPyPy; 
   std::string outputDir;
   std::string outputNameC;
   std::string outputNameH;
   std::string varPrefix;
   std::string prefix;
   int       errorCount;
   int       groupCount;
   int       allowEscapes;
   int       allowQuotes;
   xmlChar * percent;
   xmlChar * language;
   int       using_cs;
   std::string cs_filename;
   std::string cs_enum;
   std::string cs_namespace;
   int       using_py;
   int       using_py_extended;
   std::string py_dirname;
   std::string py_filename;
   int       build_dll;
};

typedef struct errp_common errp_common;

extern const char * g_functionName;

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

/* Prototypes of text.cpp functions */

void buildPath( std::string & filename, 
                std::string & dir, 
                std::string & name );

void prettify( std::string & inStr, 
               std::string & prefix,
               std::string & outStr,
               size_t        lineLength );

void stripText( xmlChar * inStr, std::string & outStr );

bool isAsciiStr( const char * str, size_t len );

xmlChar * escapeUnescapedQuotes( xmlChar * inStr );
int hasUnescapedQuotes( xmlChar * str );
void hasEscapeSequence( errp_common * commonArgs, xmlChar * str );
xmlChar * replaceChar( xmlChar * inStr, xmlChar old_c, xmlChar * new_c );

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

#endif /* ERRP_PARSER_H */
