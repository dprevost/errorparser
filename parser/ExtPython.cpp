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

#include <iostream>
#include "ExtPython.h"
#include "parser1.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

ExtPython::ExtPython( std::string & dirname,
                      std::string & header )
   : HeaderHandler( header )
{
   string name;
   
   buildPath( dirname, header, name );
   
   outStream.open( name.c_str(), fstream::out );
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ExtPython::addTopCode()
{
   outStream << "#include \"Python.h\"" << endl << endl;

   outStream << "PyObject * AddErrors(void)" << endl << "{" << endl;
   outStream << "    PyObject * errors = NULL, * errorNames = NULL, * value = NULL, * key = NULL;" << endl;
   outStream << "    int errcode;" << endl << endl;
   outStream << "    errors = PyDict_New();" << endl;
   outStream << "    if ( errors == NULL ) return NULL;" << endl << endl;
   outStream << "    errorNames = PyDict_New();" << endl;
   outStream << "    if ( errorNames == NULL ) {" << endl;
   outStream << "        Py_DECREF(errors);" << endl;
   outStream << "        return NULL;" << endl;
   outStream << "    }" << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ExtPython::addError( const std::string & errNumber,
                          const std::string & errName,
                          xmlNode           * messageNode )
{
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
   outStream << "    return Py_BuildValue(\"OO\", errors, errorNames);" << endl;
   outStream << "}" << endl << endl;
   outStream << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

