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

#include "HeaderHandler.h"
#include <string>

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

HeaderHandler::HeaderHandler( std::string  & header )
{
   unsigned int i;
   
   guard = "";
   for ( i = 0; i < header.length(); ++i ) {
      if ( isalnum(header[i]) ) {
         if ( isalpha(header[i]) ) {
            guard += toupper(header[i]);
         }
         else guard += header[i];
      }
      else {
         guard += '_';
      }
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void HeaderHandler::startHeaderGuard()
{
   outStream << "#ifndef " << guard << endl;
   outStream << "#define " << guard << endl << endl;
   outStream << "#ifdef __cplusplus" << endl;
   outStream << "extern \"C\" {" <<endl;
   outStream << "#endif" << endl << endl;
   outStream << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void HeaderHandler::stopHeaderGuard()
{
   outStream << "#ifdef __cplusplus" << endl;
   outStream << "}" << endl;
   outStream << "#endif" << endl << endl;
   outStream << "#endif /* " << guard << " */" << endl << endl;
   outStream << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

