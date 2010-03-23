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

#ifndef JAVA_H
#define JAVA_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#include "CfamilyHandler.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/** 
 * This class implements code generation for Java. 
 *
 * It extend the abstract class CFamilyHandler (which implements all 
 * functions shared by derivatives of the C language (for example,
 * adding a Copyright notice to the output using C style comments).
 */

class Java: public CfamilyHandler {
   
public:
   
   Java( bool          useTimestamp,
         std::string & filename,
         std::string & java_package,
         std::string & java_enum,
         bool          javadocStyle );
   
   // Do-nothing destructor
   ~Java() {}
   
   // Do-nothing function
   void startHeaderGuard() {}
   
   void addTopCode();
   
   // Do-nothing function
   void addGroupIdent( GroupIdent & ident ) {}
   
   void addError( ErrorXML & error,
                  bool       lastError ) throw( MissingException );

   void addBottomCode();

   // Do-nothing function
   void stopHeaderGuard() {}
   
private:
   
   std::string my_package;
   std::string my_enum;
   
   bool my_javadocStyle;
};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // JAVA_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
