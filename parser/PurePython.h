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

#ifndef PURE_PYTHON_H
#define PURE_PYTHON_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#include "AbstractHandler.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/** 
 * This class implement code generation for pure Python.
 *
 * It extend the abstract class abstractHandler by implementing all of
 * its pure virtual methods.
 */ 
class PurePython: public AbstractHandler {
   
public:

   PurePython( std::string & filename, std::string & functionName );

   // Do-nothing destructor
   ~PurePython() {}
   
   void addTop( std::string & xmlFilename,
                const char  * timeBuf,
                const char  * version );
   
   void addCopyright( Copyright & copy ) throw( MissingException );
   
   void addEndTop();

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

   std::string functionName;

};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // PURE_PYTHON_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
