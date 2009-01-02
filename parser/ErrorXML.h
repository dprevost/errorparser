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

#ifndef ERROR_XML_H
#define ERROR_XML_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#include "parser.h"

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

/** 
 * This class encapsulates the exact syntax used in the XML error file.
 *
 * This way, the rest of the code is protected against changes to
 * the DTD (if a change is needed).
 */

class ErrorXML {
   
public:
   
   ErrorXML( std::string & language,
             xmlNode     * errorNode );
   
   std::string & GetErrNumber() {
      return errorNumber;
   }

   std::string & GetErrName() {
      return errorName;
   }
   
   xmlChar * GetErrMessage();

   xmlChar * GetDocuParagraph();
   
private:
   
   std::string errorNumber;

   std::string errorName;

   xmlNode * messageNode;

   xmlNode * iterator;
   
   void GetMessageNode( std::string & language,
                        xmlNode     * message_group );
};

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

#endif // ERROR_XML_H

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
