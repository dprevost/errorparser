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

#include "ErrMessageHeader.h"
#include "parser1.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

ErrMessageHeader::ErrMessageHeader( string & dirname,
                                    string & filename,
                                    string & prefix,
                                    bool     exportFunc )
   : HeaderHandler   ( filename ),
     varPrefix       ( prefix ),
     exportFunctions ( exportFunc )
{
   string name;
   
   buildPath( dirname, filename, name );
   
   out_stream.open( name.c_str(), fstream::out );
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrMessageHeader::addBottomCode()
{
   if ( exportFunctions ) {
      /* Windows export/import decoration. */
      out_stream << "#if defined(WIN32)" << endl;
      out_stream << "#  if defined(BULDING_ERROR_MESSAGE)" << endl;
      out_stream << "#    define ERROR_MESSAGE_EXPORT __declspec ( dllexport )" << endl;
      out_stream << "#  else" << endl;
      out_stream << "#    define ERROR_MESSAGE_EXPORT __declspec ( dllimport )" << endl;
      out_stream << "#  endif" << endl;
      out_stream << "#else" << endl;
      out_stream << "#  define ERROR_MESSAGE_EXPORT" << endl;
      out_stream << "#endif" << endl << endl;
      out_stream << barrier << endl << endl;
   }
   
   out_stream << "/*" << endl;
   out_stream << " * Use this function to access the error messages (defined in the xml" << endl;
   out_stream << " * input file)." << endl;
   out_stream << " * " << endl;
   out_stream << " * Parameters:" << endl;
   out_stream << " *   - errnum    The error number" << endl;
   out_stream << " *" << endl;
   out_stream << " * Return values:" << endl;
   out_stream << " *   - the error message if errnum is valid (exists)" << endl;
   out_stream << " *   - NULL otherwise" << endl;
   out_stream << " */" << endl;

   if ( exportFunctions ) out_stream << "ERROR_MESSAGE_EXPORT" << endl;
   out_stream << "const char * " << varPrefix << "_ErrorMessage( int errnum );" << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
