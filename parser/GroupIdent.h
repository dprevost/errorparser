/* -*- c++ -*- */
/* :mode=c++:  - For jedit, previous line for emacs */
/*
 * Copyright (C) 2008-2010 Daniel Prevost <dprevost@users.sourceforge.net>
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

#ifndef GROUP_IDENT_H
#define GROUP_IDENT_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>

#include "parser.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/** 
 * This class encapsulates the exact syntax used in the XML error file
 * for the tag "errgroup_ident".
 *
 * This way, the rest of the code is protected against changes to
 * the DTD (if changes are needed).
 */

class GroupIdent {
   
public:
   
   // identNode is passed by reference - it will be updated by this ctor
   // to the proper place in the tree so that the caller can just reuse it
   // to continue parsing the xml tree.
   GroupIdent( std::string  & language,
               xmlNode     *& identNode ) throw( MissingException );
   
   std::string & GetName() {
      return name;
   }

   // Returns NULL at the end
   const char * GetDescParagraph();
   
private:
   
   std::string name;

   xmlNode * textNode;

   xmlNode * iterator;
   
};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // GROUP_IDENT_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
