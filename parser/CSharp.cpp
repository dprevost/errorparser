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

#include "CSharp.h"
#include "parser1.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

CSharp::CSharp( string & filename, string & cs_namespace, string & cs_enum )
   : my_namespace( cs_namespace ),
     my_enum     ( cs_enum ),
     indent      ( "" )
{
   out_stream.open( filename.c_str(), fstream::out );
   
   if ( my_namespace.length() > 0 ) indent = "    ";
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void CSharp::addTopCode()
{
   out_stream << "using System;" << endl << endl;

   if ( my_namespace.length() > 0 ) {
      out_stream << "namespace " << my_namespace << endl << "{" << endl;
   }
   
   out_stream << indent << "public enum " << my_enum << endl;
   out_stream << indent << "{" << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void CSharp::addBottomCode()
{
   out_stream << indent << "}" << endl;
   if ( my_namespace.length() > 0 ) {
      out_stream << "}" << endl;
   }
   out_stream << endl << barrier << endl << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
