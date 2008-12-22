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

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

HeaderHandler::HeaderHandler( std::string  & header )
{
   unsigned int i;
   
   guard = "";
   for ( i = 0; i < header.length(); ++i ) {
      if ( isalnum(header[i]) ) {
         if ( isalpha(header[i]) ) {
            guard[i] += toupper(header[i]);
         }
         else guard[i] += header[i];
      }
      else {
         guard[i] += '_';
      }
   }

}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void HeaderHandler::startHeaderGuard()
{
   out_stream << "#ifndef " << guard << endl;
   out_stream << "#define " << guard << endl << endl;
   out_stream << "#ifdef __cplusplus" << endl;
   out_stream << "extern \"C\" {" <<endl;
   out_stream << "#endif" << endl << endl;
   out_stream << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void HeaderHandler::stopHeaderGuard()
{
   out_stream << barrier << endl << endl;
   out_stream << "#ifdef __cplusplus" << endl;
   out_stream << "}" << endl;
   out_stream << "#endif" << endl << endl;
   out_stream << "#endif /* " << guard << " */" << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

