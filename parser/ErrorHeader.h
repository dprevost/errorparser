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

#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "HeaderHandler.h"
#include <string>

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/** 
 * This class extend the abstract class abstractHandler - it generates
 * code for the C header file holding the error numbers.
 */ 
class ErrorHeader: public HeaderHandler{
   
public:
   ErrorHeader( std::string & dir,
                std::string & header,
                std::string & enumName,
                std::string & prefix );
   
   // Do-nothing destructor
   ~ErrorHeader() {}
   
   void addTopCode();
   
   void addGroupIdent( GroupIdent & ident ) throw( MissingException );

   void addError( ErrorXML & error,
                  bool lastError ) throw( MissingException );

   void addBottomCode();

private:
   
   std::string enumName;

   std::string prefix;
   
   bool usingEnum;

};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // ERROR_HANDLER_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

