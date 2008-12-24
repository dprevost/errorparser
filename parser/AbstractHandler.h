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

#ifndef ABSTRACT_HANDLER_H
#define ABSTRACT_HANDLER_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#if defined WIN32
#  define _CRT_SECURE_NO_DEPRECATE
#  include "config-win32.h"
   /* 
    * The pragma is to hide a warning in Microsoft include files
    * with VC++ 6. Don't know about other versions yet.
    */
#  pragma warning(disable:4115)
#  include <windows.h>
#  pragma warning(default:4115)
#  include <winbase.h>
#  include <io.h>
#  include <process.h> 
#endif

#include <fstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

class AbstractHandler {
   
public:
   virtual ~AbstractHandler() {}
   
   virtual void addTop( std::string & xmlFilename,
                        char        * timeBuf,
                        xmlChar     * version ) = 0;
   
   virtual void addCopyright( xmlNode * node ) = 0;
   
   virtual void addEndTop() = 0;
   
   virtual void startHeaderGuard() = 0;
   
   virtual void addTopCode() = 0;
   
   virtual void addGroupName( std::string name ) = 0;
   
   virtual void addGroupDesc( std::string description ) = 0;

   virtual void endGroupDesc() = 0;
   
   virtual void addError() = 0;
   
   virtual void addBottomCode() = 0;
   
   virtual void stopHeaderGuard() = 0;
   
protected:
   
   AbstractHandler() {}
   
   std::fstream out_stream;

   std::string barrier;
};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // ABSTRACT_HANDLER_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

