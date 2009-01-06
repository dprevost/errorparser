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
#  include <windows.h>
#endif

#include <time.h>
#include <string>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#define ERRP_LINE_LENGTH 72

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

// Prototypes of text.cpp functions.

void buildPath( std::string & dirname, 
                std::string & filename, 
                std::string & name );

void prettify( std::fstream      & outStream,
               const std::string & inStr, 
               const std::string & prefix, 
               size_t              lineLength );

void stripText( xmlChar * inStr, std::string & outStr );

bool isAsciiStr( const char * str, size_t len );

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // ERRP_PARSER_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

