/* -*- c++ -*- */
/* :mode=c++:  - For jedit, previous line for emacs */
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

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#ifndef EXT_PYTHON_H
#define EXT_PYTHON_H

#include <string>
#include "HeaderHandler.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/** 
 * This class extend the abstract class HeaderHandler - it generates
 * code for Python extension modules.
 */ 
class ExtPython: public HeaderHandler{
   
public:
   ExtPython( std::string & dir,
              std::string & header );
   
   void addTopCode();
   
   void addGroupName( std::string name ) {}
   
   void addGroupDesc( std::string description ) {}

   void endGroupDesc() {}

   void addError( const std::string & errNumber, 
                  const std::string & errName, 
                  xmlNode           * messageNode );
   
   void addErrorTrailer() {}
   
   void addBottomCode();
   
};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // EXT_PYTHON_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

