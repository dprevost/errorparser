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

#ifndef EXT_PYTHON_H
#define EXT_PYTHON_H

#include "HeaderHandler.h"
#include <string>

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/** 
 * This class extend the abstract class HeaderHandler - it generates
 * code for Python extension modules (a header file that can be included
 * in an extension module).
 */ 
class ExtPython: public HeaderHandler {
   
public:
   
   // If "function" is empty (""), the code uses AddErrors()
   ExtPython( std::string & dir,
              std::string & header,
              std::string & function );
   
   void addTopCode();
   
   void addGroupIdent( GroupIdent & ident ) {}

   void addError( ErrorXML & error, bool lastError );

   void addBottomCode();

private:
   
   std::string functionName;
};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // EXT_PYTHON_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

