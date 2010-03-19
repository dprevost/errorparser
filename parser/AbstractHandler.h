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

#ifndef ABSTRACT_HANDLER_H
#define ABSTRACT_HANDLER_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#include "parser.h"

#include <fstream>

#include "ErrorXML.h"
#include "Copyright.h"
#include "GroupIdent.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

class AbstractHandler {
   
public:

   // Do-nothing constructor
   AbstractHandler() {}

   // Do-nothing destructor
   virtual ~AbstractHandler() {}
   
   // This method adds code to the beginning of the file. For example, that
   // the file was generated by Error Parser, a time stamp, etc. 
   virtual void addTop( std::string & xmlFilename,
                        const char  * timeBuf,
                        const char  * version ) = 0;
   
   // Adds copyright notices to the generated files, if requested 
   // (copyright notices are optional - error parser supports multiple
   // copyright notices as needed).
   virtual void addCopyright( Copyright & copy ) = 0;
   
   // Ends the top portion of the generated file. This method will often
   // simply close a comment or some other trivial task.
   virtual void addEndTop() = 0;
   
   // Add a guard to C/C++ header files
   virtual void startHeaderGuard() = 0;
   
   // Usually this will be the function and variable declarations, if any
   virtual void addTopCode() = 0;
   
   // This method is used to add documentation for group of errors when
   // this feature is used.
   virtual void addGroupIdent( GroupIdent & ident ) = 0;
   
   // Called for each error in the xml file.
   virtual void addError( ErrorXML & error, bool lastError ) = 0;
   
   // Close the function declared with addTopCode(), if any, and add cleanup
   // and anything else needed.
   virtual void addBottomCode() = 0;
   
   // Close the guard (for C/C++ header files).
   virtual void stopHeaderGuard() = 0;
   
protected:
   
   std::fstream outStream;

   // A comment "barrier" to separate "sections" of the generated code.
   std::string barrier;
};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // ABSTRACT_HANDLER_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

