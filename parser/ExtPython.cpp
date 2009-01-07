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

#include "ExtPython.h"
#include "parser.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

ExtPython::ExtPython( string & dirname,
                      string & header,
                      string & function )
   : HeaderHandler( header ),
     functionName ( function )
{
   string name;
   
   buildPath( dirname, header, name );
   
   outStream.open( name.c_str(), fstream::out );
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ExtPython::addTopCode()
{
   outStream << "#include \"Python.h\"" << endl << endl;

   // Declare our function and the needed variables.
   if ( functionName.length() == 0 ) {
      outStream << "PyObject * AddErrors(void)" << endl << "{" << endl;
   }
   else {
      outStream << "PyObject * " << functionName << "(void)" << endl << "{" << endl;
   }

   outStream << "    PyObject * errors = NULL, * errorNames = NULL, * value = NULL, * key = NULL;" << endl;
   outStream << "    int errcode;" << endl << endl;

   // And create our 2 python dict (associated arrays)
   outStream << "    errors = PyDict_New();" << endl;
   outStream << "    if ( errors == NULL ) return NULL;" << endl << endl;
   outStream << "    errorNames = PyDict_New();" << endl;
   outStream << "    if ( errorNames == NULL ) {" << endl;
   outStream << "        Py_DECREF(errors);" << endl;
   outStream << "        return NULL;" << endl;
   outStream << "    }" << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ExtPython::addError( ErrorXML & error, bool lastError )
{
   bool firstpara = true;
   string tmp;
   string & errNumber = error.GetErrNumber();
   string & errName   = error.GetErrName();
   const char * paragraph;
   
   outStream << "    /*" << endl;

   paragraph = error.GetDocuParagraph();
   while ( paragraph != NULL ) {
      stripText( paragraph, tmp );
         
      if ( firstpara ) firstpara = false;
      else outStream << "     *" << endl;

      formatText( outStream, tmp, "     * ", ERRP_LINE_LENGTH );
      
      paragraph = error.GetDocuParagraph();
   }

   outStream << "     */" << endl;

   // The following generated code inserts values in the two dict. It is
   // a bit convoluted because of error handling and the need to decrease
   // the ref. counters properly (when something goes wrong).
   outStream << "    value = PyInt_FromLong(" << errNumber << ");" << endl;
   outStream << "    if ( value == NULL ) {" << endl;
   outStream << "        PyDict_Clear(errors);" << endl;
   outStream << "        Py_DECREF(errors);" << endl;
   outStream << "        PyDict_Clear(errorNames);" << endl;
   outStream << "        Py_DECREF(errorNames);" << endl;
   outStream << "        return NULL;" << endl;
   outStream << "    }" << endl;
   outStream << "    key = PyString_FromString(\"" << errName << "\");" << endl;
   outStream << "    if ( key == NULL ) {" << endl;
   outStream << "        Py_DECREF(value);" << endl;
   outStream << "        PyDict_Clear(errors);" << endl;
   outStream << "        Py_DECREF(errors);" << endl;
   outStream << "        PyDict_Clear(errorNames);" << endl;
   outStream << "        Py_DECREF(errorNames);" << endl;
   outStream << "        return NULL;" << endl;
   outStream << "    }" << endl;
   outStream << "    errcode = PyDict_SetItem(errors, key, value);" << endl;
   outStream << "    if (errcode != 0) {" << endl;
   outStream << "        Py_DECREF(value);" << endl;
   outStream << "        Py_DECREF(key);" << endl;
   outStream << "        PyDict_Clear(errors);" << endl;
   outStream << "        Py_DECREF(errors);" << endl;
   outStream << "        PyDict_Clear(errorNames);" << endl;
   outStream << "        Py_DECREF(errorNames);" << endl;
   outStream << "        return NULL;" << endl;
   outStream << "    }" << endl;
   outStream << "    errcode = PyDict_SetItem(errorNames, value, key);" << endl;
   outStream << "    Py_DECREF(value);" << endl;
   outStream << "    Py_DECREF(key);" << endl;
   outStream << "    if (errcode != 0) {" << endl;
   outStream << "        PyDict_Clear(errors);" << endl;
   outStream << "        Py_DECREF(errors);" << endl;
   outStream << "        PyDict_Clear(errorNames);" << endl;
   outStream << "        Py_DECREF(errorNames);" << endl;
   outStream << "        return NULL;" << endl;
   outStream << "    }" << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ExtPython::addBottomCode()
{
   // Return our two dict as a tuple to the calling function.
   outStream << "    return Py_BuildValue(\"OO\", errors, errorNames);" << endl;
   outStream << "}" << endl << endl;
   outStream << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

