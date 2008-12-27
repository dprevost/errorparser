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
   
   out_stream.open( name.c_str(), fstream::out );
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ExtPython::addTopCode()
{
   out_stream << "#include \"Python.h\"" << endl << endl;

   out_stream << "PyObject * AddErrors(void)" << endl << "{" << endl;
   out_stream << "    PyObject * errors = NULL, * errorNames = NULL, * value = NULL, * key = NULL;" << endl;
   out_stream << "    int errcode;" << endl << endl;
   out_stream << "    errors = PyDict_New();" << endl;
   out_stream << "    if ( errors == NULL ) return NULL;" << endl << endl;
   out_stream << "    errorNames = PyDict_New();" << endl;
   out_stream << "    if ( errorNames == NULL ) {" << endl;
   out_stream << "        Py_DECREF(errors);" << endl;
   out_stream << "        return NULL;" << endl;
   out_stream << "    }" << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ExtPython::addError( const std::string & errNumber,
                          const std::string & errName,
                          xmlNode           * messageNode )
{
   out_stream << "    value = PyInt_FromLong(" << errNumber << ");" << endl;
   out_stream << "    if ( value == NULL ) {" << endl;
   out_stream << "        PyDict_Clear(errors);" << endl;
   out_stream << "        Py_DECREF(errors);" << endl;
   out_stream << "        PyDict_Clear(errorNames);" << endl;
   out_stream << "        Py_DECREF(errorNames);" << endl;
   out_stream << "        return NULL;" << endl;
   out_stream << "    }" << endl;
   out_stream << "    key = PyString_FromString(\"" << errName << "\");" << endl;
   out_stream << "    if ( key == NULL ) {" << endl;
   out_stream << "        Py_DECREF(value);" << endl;
   out_stream << "        PyDict_Clear(errors);" << endl;
   out_stream << "        Py_DECREF(errors);" << endl;
   out_stream << "        PyDict_Clear(errorNames);" << endl;
   out_stream << "        Py_DECREF(errorNames);" << endl;
   out_stream << "        return NULL;" << endl;
   out_stream << "    }" << endl;
   out_stream << "    errcode = PyDict_SetItem(errors, key, value);" << endl;
   out_stream << "    if (errcode != 0) {" << endl;
   out_stream << "        Py_DECREF(value);" << endl;
   out_stream << "        Py_DECREF(key);" << endl;
   out_stream << "        PyDict_Clear(errors);" << endl;
   out_stream << "        Py_DECREF(errors);" << endl;
   out_stream << "        PyDict_Clear(errorNames);" << endl;
   out_stream << "        Py_DECREF(errorNames);" << endl;
   out_stream << "        return NULL;" << endl;
   out_stream << "    }" << endl;
   out_stream << "    errcode = PyDict_SetItem(errorNames, value, key);" << endl;
   out_stream << "    Py_DECREF(value);" << endl;
   out_stream << "    Py_DECREF(key);" << endl;
   out_stream << "    if (errcode != 0) {" << endl;
   out_stream << "        PyDict_Clear(errors);" << endl;
   out_stream << "        Py_DECREF(errors);" << endl;
   out_stream << "        PyDict_Clear(errorNames);" << endl;
   out_stream << "        Py_DECREF(errorNames);" << endl;
   out_stream << "        return NULL;" << endl;
   out_stream << "    }" << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ExtPython::addBottomCode()
{
   out_stream << "    return Py_BuildValue(\"OO\", errors, errorNames);" << endl;
   out_stream << "}" << endl << endl;
   out_stream << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
