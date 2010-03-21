/* -*- c++ -*- */
/* :mode=c++:  - For jedit, previous line for emacs */
/*
 * Copyright (C) 2009-2010 Daniel Prevost <dprevost@users.sourceforge.net>
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

#ifndef MESSAGE_XML_H
#define MESSAGE_XML_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>

#include "parser.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/** 
 * This class handles the transformation of the original string 
 * taken from the input xml file into a "safe" string that can be
 * used by C/C++/Java programs.
 */

class MessageXML {
   
public:
   
   MessageXML( bool          allowEscapes,
               bool          allowQuotes,
               std::string & percent );
   
   void GetErrMessage( const char  * nodeMsg,
                       std::string & message ) throw( MissingException );

private:
   
   bool allowEscapes;
   bool allowQuotes;
   std::string percent;
   
   void hasEscapeSequence( std::string & str );

   bool hasUnescapedQuotes( std::string & str );

   void escapeUnescapedQuotes( const std::string & inStr, 
                               std::string       & outStr );
   
   void stripPercent( const std::string & inStr, 
                      std::string       & outStr );

};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // MESSAGE_XML_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
