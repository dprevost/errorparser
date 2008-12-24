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

#ifndef ERR_MESSAGE_HEADER_H
#define ERR_MESSAGE_HEADER_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#include "HeaderHandler.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/** 
 * This class implements code generation for the header file used to
 * declare the C file holding the error messages (allowing a project to 
 * retrieve these messages using the error code).
 *
 * It extend the abstract class HeaderHandler (which implements all 
 * functions shared by header files (guard generation, for example).
 */

class ErrMessageHeader:  public HeaderHandler {
   
public:
   
   ErrMessageHeader( std::string & dirname,
                     std::string & filename,
                     std::string & prefix,
                     bool          exportFunc );
   
   void addTopCode() {}
   
   void addGroupName( std::string name ) {}
   
   void addGroupDesc( std::string description ) {}

   void endGroupDesc() {}

   void addError() {}

   void addBottomCode();
   
private:
   
   std::string varPrefix;

   bool exportFunctions;
   
};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // ERR_MESSAGE_HEADER_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
