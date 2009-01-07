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

// Microsoft specific. Must be defined before including system header
// files (this will avoid a warning if we ever use a C library function 
// declared by Microsoft as deprecated.
#define _CRT_SECURE_NO_DEPRECATE

#include <string>
#include <fstream>
#include <iostream>

#include "ErrMessageHeader.h"
#include "parser.h"

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
   
   outStream.open( name.c_str(), fstream::out );
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrMessageHeader::addBottomCode()
{
   if ( exportFunctions ) {
      // Windows export/import decoration.
      outStream << "#if defined(WIN32)" << endl;
      outStream << "#  if defined(BULDING_ERROR_MESSAGE)" << endl;
      outStream << "#    define ERROR_MESSAGE_EXPORT __declspec ( dllexport )" << endl;
      outStream << "#  else" << endl;
      outStream << "#    define ERROR_MESSAGE_EXPORT __declspec ( dllimport )" << endl;
      outStream << "#  endif" << endl;
      outStream << "#else" << endl;
      outStream << "#  define ERROR_MESSAGE_EXPORT" << endl;
      outStream << "#endif" << endl << endl;
      outStream << barrier << endl << endl;
   }
   
   outStream << "/*" << endl;
   outStream << " * Use this function to access the error messages (defined in the xml" << endl;
   outStream << " * input file)." << endl;
   outStream << " * " << endl;
   outStream << " * Parameters:" << endl;
   outStream << " *   - errnum    The error number" << endl;
   outStream << " *" << endl;
   outStream << " * Return values:" << endl;
   outStream << " *   - the error message if errnum is valid (exists)" << endl;
   outStream << " *   - NULL otherwise" << endl;
   outStream << " */" << endl;

   if ( exportFunctions ) outStream << "ERROR_MESSAGE_EXPORT" << endl;
   outStream << "const char * " << varPrefix << "_ErrorMessage( int errnum );" << endl << endl;

   outStream << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

