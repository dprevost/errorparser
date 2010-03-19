/* -*- c++ -*- */
/* :mode=c++:  - For jedit, previous line for emacs */
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

#ifndef COPYRIGHT_H
#define COPYRIGHT_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>

#include "parser.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/** 
 * This class encapsulates the exact syntax used in the XML error file
 * for the copyright_group.
 *
 * This way, the rest of the code is protected against changes to
 * the DTD (if a change is needed).
 */

class Copyright {
   
public:
   
   Copyright( xmlNode * copyNode ) throw( MissingException );
   
   std::string & GetAuthors() {
      return authors;
   }

   std::string & GetYears() {
      return years;
   }
   
   const char * GetCopyParagraph();
   
private:
   
   std::string authors;

   std::string years;

   xmlNode * textNode;

   xmlNode * iterator;
   
};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // COPYRIGHT_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
