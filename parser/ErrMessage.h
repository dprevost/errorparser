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

#ifndef ERR_MESSAGE_H
#define ERR_MESSAGE_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#include "CfamilyHandler.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/** 
 * This class implements code generation for the C file holding the error
 * messages (allowing a project to retrieve these messages using the 
 * error code).
 *
 * It extend the abstract class CFamilyHandler (which implements all 
 * functions shared by derivatives of the C language (for example,
 * adding a Copyright notice to the output using C style comments).
 */

class ErrMessage:  public CfamilyHandler {
   
public:
   
   ErrMessage( bool          useTimestamp,
               std::string & filename,
               std::string & header,
               std::string & prefix,
               std::string & varPrefix );
   
   // Do-nothing destructor
   void startHeaderGuard() {}
   
   void addTopCode();
   
   // Do-nothing function
   void addGroupIdent( GroupIdent & ident ) {}

   void addError( ErrorXML & error, bool lastError );
   
   void addBottomCode();
   
   // Do-nothing function
   void stopHeaderGuard() {}
   
private:
   
   int errorCount;
   std::string headerName;
   std::string prefix;
   std::string varPrefix;

};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // ERR_MESSAGE_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
