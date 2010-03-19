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

#ifndef ERRP_PARSER_H
#define ERRP_PARSER_H

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#if defined(WIN32)
#  include "config-win32.h"
#endif

#include <time.h>
#include <string>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>

// char can be unsigned on some systems. If this occurs, the two stripText()
// will have the same signature -> compiler error.
#include <limits.h>
#if CHAR_MIN == 0
#  define CHAR_UNSIGNED 1
#endif

#include "MissingException.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

// You could override this macro for a specic output handler by calling 
// formatText() with a different value. If you do so, your handler should
// be inherited directly from AbstractHandler and not one of its derived
// classes since the derived classes will likely use ERRP_LINE_LENGTH
// when calling formatText().
#define ERRP_LINE_LENGTH 72

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

//#include "MessageXML.h"
class MessageXML;

extern MessageXML * g_msgXML;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

// Prototypes of text.cpp functions.

void buildPath( std::string & dirname, 
                std::string & filename, 
                std::string & name );

void formatText( std::fstream      & outStream,
                 const std::string & inStr, 
                 const std::string & prefix, 
                 size_t              lineLength );

void stripText( const char * inStr, std::string & outStr );

#if ! defined(CHAR_UNSIGNED)
inline 
void stripText( xmlChar * inStr, std::string & outStr )
{
   return stripText( (const char *)inStr, outStr );
}
#endif

bool isAsciiStr( const char * str, size_t len );

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // ERRP_PARSER_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

