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

#include "HeaderHandler.h"
#include <string>
#include <iostream>

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

HeaderHandler::HeaderHandler( std::string  & header )
{
   unsigned int i;

   // The first character of a C identifier must be a letter or the
   // underscore ('_'). The underscore is not recommended, however.
   if ( isalpha(header[0]) ) {
      guard = toupper(header[0]);
   }
   else {
      guard = '_';
      if ( isdigit(header[0]) ) {
         cerr << "Warning: C identifiers cannot start with a digit (I've replaced it with '_')" << endl;
      }
      else if ( header[0] == '_' ) {
         cerr << "Warning: Starting a C identifier with '_' might conflict with identifiers from the C runtime library" << endl;
      }
      else {
         cerr << "Warning: invalid character for a C identifier (I've replaced it with '_')" << endl;
      }
   }
   
   // Put all letters in uppercase and set non-alphanumeric values
   // to the underscore ('_').
   for ( i = 1; i < header.length(); ++i ) {
      if ( isalnum(header[i]) ) {
         if ( isalpha(header[i]) ) {
            guard += toupper(header[i]);
         }
         else guard += header[i];
      }
      else {
         guard += '_';
         if ( header[i] != '_' && header[i] != '\\' && header[i] != '/' && header[i] != '.' ) {
            cerr << "Warning: invalid character for a C identifier (I've replaced it with '_')" << endl;
         }
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

