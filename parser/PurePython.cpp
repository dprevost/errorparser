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

// Microsoft specific. Must be defined before including system header
// files (this will avoid a warning if we ever use a C library function 
// declared by Microsoft as deprecated.
#define _CRT_SECURE_NO_DEPRECATE

#include <string>
#include <fstream>
#include <iostream>

#include "PurePython.h"
#include "parser.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

PurePython::PurePython( bool     useTimestamp,
                        string & filename,
                        string & functionName )
   : AbstractHandler ( useTimestamp ),
     functionName    (functionName)
{
   outStream.exceptions( fstream::failbit | fstream::badbit);
   outStream.open( filename.c_str(), fstream::out );
   
   barrier = "# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--";
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void PurePython::addTop( string     & xmlFilename,
                         const char * timeBuf,
                         const char * version )
{
   outStream << "# This file was generated by the program errorParser" << endl;
   outStream << "# using the input file " << xmlFilename << "." << endl;
   if ( writeTimestamp ) {
      outStream << "# Date: " << timeBuf << "." << endl << endl;
   }
   if ( version != NULL ) {
      outStream << "# The version of the XML error file is " << 
         version << "." << endl << endl;
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void PurePython::addCopyright( Copyright & copy ) throw( MissingException )
{
   string & years   = copy.GetYears();
   string & authors = copy.GetAuthors();
   string tmp, prefix = "# ";
   const char * paragraph;
   
   outStream << "# Copyright (C) " << years << " " << authors << endl;
   
   paragraph = copy.GetCopyParagraph();
   while ( paragraph != NULL ) {
         
         outStream << "#" << endl;

         stripText( paragraph, tmp );
         if ( tmp.empty() ) throw new MissingException( "<license_para>" );
         formatText( outStream, tmp, prefix, ERRP_LINE_LENGTH );
         
      paragraph = copy.GetCopyParagraph();
   }

   outStream << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void PurePython::addEndTop()
{
   outStream << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void PurePython::addTopCode()
{
   if ( functionName.length() == 0 ) {
      outStream << "def add_errors():" << endl << endl;
   }
   else {
      outStream << "def " << functionName << "():" << endl << endl;
   }
   outStream << "    errors = dict()" << endl;
   outStream << "    error_names = dict()" << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void PurePython::addError( ErrorXML & error,
                           bool       lastError ) throw( MissingException )
{
   bool firstpara = true;
   string tmp;
   string & errNumber = error.GetErrNumber();
   string & errName   = error.GetErrName();
   const char * paragraph;
   
   paragraph = error.GetDocuParagraph();
   while ( paragraph != NULL ) {
      stripText( paragraph, tmp );
      if ( tmp.empty() ) throw new MissingException( "<errordoc>" );
      
      if ( firstpara ) firstpara = false;
      else {
         outStream << "    #" << endl;
      }
      formatText( outStream, tmp, "    # ", ERRP_LINE_LENGTH );
      
      paragraph = error.GetDocuParagraph();
   }

   outStream << "    errors['" << errName << "'] = " << errNumber << endl;
   outStream << "    error_names[" << errNumber << "] = '" << errName << "'" << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void PurePython::addBottomCode()
{
   outStream << endl;
   outStream << "    return tuple(errors, error_names)" << endl << endl;
   outStream << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

