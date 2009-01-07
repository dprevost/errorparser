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

#ifndef HEADER_HANDLER_H
#define HEADER_HANDLER_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#include "CfamilyHandler.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/** 
 * This class is abstract. It extend the abstract class CfamilyHandler for
 * C header files. 
 *
 * Concretely, it adds the two functions needed to write the header guard.
 */ 
class HeaderHandler: public CfamilyHandler {
   
public:

   // Do-nothing destructor
   virtual ~HeaderHandler() {}
   
   // Additionally, this function adds the extern "C" statement (for C++)":
   //    #ifdef __cplusplus
   //    extern "C" {
   //    #endif
   virtual void startHeaderGuard();
   
   // Additionally, this function closes the extern "C" statement (for C++)
   virtual void stopHeaderGuard();
   
protected:
   
   // The guard is constructed from the headerName.
   // Invalid characters in headerName will generate warnings (the invalid
   // characters themselves will be replaced with underscores.
   HeaderHandler( std::string & headerName );
   
   std::string guard;

};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // HEADER_HANDLER_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
