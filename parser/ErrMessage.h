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
   
   ErrMessage( std::string & filename,
               std::string & header,
               std::string & prefix,
               std::string & varPrefix,
               bool          allowEscapes,
               bool          allowQuotes,
               std::string & percent );
   
   void startHeaderGuard() {}
   
   void addTopCode();
   
   void addGroupIdent( GroupIdent & ident ) {}

   void addError( ErrorXML & error, bool lastError );
   
   void addBottomCode();
   
   void stopHeaderGuard() {}
   
private:
   
   int errorCount;
   std::string headerName;
   std::string prefix;
   std::string varPrefix;
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

#endif // ERR_MESSAGE_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
