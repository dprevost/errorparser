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

#ifndef CSHARP_H
#define CSHARP_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#include "CfamilyHandler.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/** 
 * This class implements code generation for C#. 
 *
 * It extend the abstract class CFamilyHandler (which implements all 
 * functions shared by derivatives of the C language (for example,
 * adding a Copyright notice to the output using C style comments).
 */

class CSharp: public CfamilyHandler {
   
public:
   
   CSharp( bool          useTimestamp,
           std::string & filename,
           std::string & cs_namespace,
           std::string & cs_enum );
   
   // Do-nothing destructor
   ~CSharp() {}
   
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
   
   std::string my_namespace;
   std::string my_enum;
   
   // If we use a namespace, we define indent to be 4 spaces, otherwise
   // it's an empty string. To make the generated code prettier.
   std::string indent;
};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // CSHARP_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
