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

#ifndef ABSTRACT_HANDLER_H
#define ABSTRACT_HANDLER_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#include "parser.h"

#include <fstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>

#include "ErrorXML.h"
#include "Copyright.h"
#include "GroupIdent.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

class AbstractHandler {
   
public:
   virtual ~AbstractHandler() {}
   
   virtual void addTop( std::string & xmlFilename,
                        char        * timeBuf,
                        xmlChar     * version ) = 0;
   
   virtual void addCopyright( Copyright & copy ) = 0;
   
   virtual void addEndTop() = 0;
   
   virtual void startHeaderGuard() = 0;
   
   virtual void addTopCode() = 0;
   
   virtual void addGroupIdent( GroupIdent & ident ) = 0;
   
   virtual void addError( ErrorXML & error ) = 0;
   
   // The trailer is added to all errors which are not the first or last of
   // a group of errors. 
   virtual void addErrorTrailer() = 0;
   
   virtual void addBottomCode() = 0;
   
   virtual void stopHeaderGuard() = 0;
   
protected:
   
   AbstractHandler() {}
   
   std::fstream outStream;

   std::string barrier;
};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // ABSTRACT_HANDLER_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

