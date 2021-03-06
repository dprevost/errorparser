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
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>

#include "parser.h"
#include "AbstractHandler.h"
#include "ErrorHeader.h"
#include "ErrMessageHeader.h"
#include "ErrMessage.h"
#include "CSharp.h"
#include "ExtPython.h"
#include "PurePython.h"
#include "Java.h"
#include "MessageXML.h"

using namespace std;

bool GetGeneralOptions( string  & prefix,
                        string  & language,
                        xmlNode * node,
                        int       version,
                        bool    & useTimestamp );
// Special code for version 2.0 of the option file
bool GetGeneralOptions20( string  & prefix,
                          string  & language,
                          xmlNode * node );

bool AddHeaderFileHandler( vector<AbstractHandler *> & handlers,
                           string                    & prefix,
                           xmlNode                   * child,
                           bool                        useTimestamp );

bool AddErrMessageHandlers( vector<AbstractHandler *> & handlers,
                            string                    & prefix,
                            xmlNode                   * node,
                            bool                        useTimestamp );

// Special code for version 2.0 of the option file
bool AddErrMessageHandlers20( vector<AbstractHandler *> & handlers,
                              string                    & prefix,
                              xmlNode                   * node,
                              bool                        useTimestamp );

bool AddCSharpHandler( vector<AbstractHandler *> & handlers,
                       xmlNode                   * node,
                       bool                        useTimestamp );

bool AddExtPython( vector<AbstractHandler *> & handlers,
                   xmlNode                   * node,
                   bool                        useTimestamp );

bool AddPurePythonHandler( vector<AbstractHandler *> & handlers,
                           xmlNode                   * node,
                           bool                        useTimestamp );

bool AddJavaHandler( vector<AbstractHandler *> & handlers,
                     xmlNode                   * node,
                     bool                        useTimestamp,
                     bool                        javadocStyle );

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

//
// Warning: in the next few helper functions, the xmlNode pointer, node, is 
// passed by reference (and therefore updated in the calling function).
//

xmlChar * GetMandatoryValue( xmlNode * & node, const char * tag )
{
   xmlChar * value = NULL;
   
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST tag) == 0 ) {
            value = node->children->content;
            node = node->next;
            break;
         }
         return NULL;
      }
      node = node->next;
   }
   
   return value;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

xmlChar * GetOptionalValue( xmlNode * & node, const char * tag )
{
   xmlChar * value = NULL;
   
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST tag) == 0 ) {
            value = node->children->content;
            node = node->next;
         }
         break; 
      }
      node = node->next;
   }

   return value;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

xmlNode * IsMandatoryValuePresent( xmlNode * & node, const char * tag )
{
   xmlNode * nodeValue = NULL;
   
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST tag) == 0 ) {
            nodeValue = node;
            node = node->next;
            break;
         }
         return NULL;
      }
      node = node->next;
   }
   
   return nodeValue;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

xmlNode * IsOptionalValuePresent( xmlNode * & node, const char * tag )
{
   xmlNode * nodeValue = NULL;
   
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST tag) == 0 ) {
            nodeValue = node;
            node = node->next;
         }
         break; 
      }
      node = node->next;
   }

   return nodeValue;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

bool handleOptions( vector<AbstractHandler *> & handlers,
                    string & xmlFileName,
                    string & xmlOptionName,
                    string & language )
{
   xmlParserCtxtPtr context = NULL;
   xmlNode * root = NULL, * node, * nodeValue;
   xmlDoc  * doc;
   int i;
   xmlChar * prop;
   int version = 0;
   string tmp;
   string prefix;
   bool useTimestamp = true;
   bool javadocStyle = false;

   context = xmlNewParserCtxt();
   if ( context == NULL ) {
      cerr << "Error allocating the parser context" << endl;
      return false;
   }

   // We create the document and validate in one go.
   doc = xmlCtxtReadFile( context, 
                          xmlOptionName.c_str(),
                          NULL,
                          XML_PARSE_DTDVALID );
   if ( doc == NULL ) {
      cerr << "Error while parsing the option file: " << xmlOptionName << endl;
      return false;
   }
   if ( context->valid == 0 ) {
      cerr << "Error: document validation (for options) failed" << endl;
      return false;
   }
   
   root = xmlDocGetRootElement( doc );

   prop = xmlGetProp( root, BAD_CAST "version" );
   if ( prop == NULL ) {
      cerr << "Error: missing \"version\" attribute to the root of the option XML" << endl;
      return false;
   }
   else {
      /*
       * Acceptable format: 2, 2., 2.0, 2.00, 2.000 (etc.)
       * We also accept the comma as the digital separator.
       *
       * But... our versions will never have more than one digit after
       * the decimal separator (so we drop additional digits).
       */
      int separator = xmlStrlen( prop );
      for ( i = 0; i < xmlStrlen( prop ); i++ ) {
         if ( prop[i] == '.' || prop[i] == ',' ) {
            separator = i;
            break;
         }
      }
      
      if ( separator < xmlStrlen( prop ) ) {
         for ( i = 0; i < separator; i++ ) {
            if ( prop[i] >= '0' && prop[i] <= '9' ) {
               version *= 10;
               version += prop[i] - '0';
            }
            else {
               cerr << "Error: invalid format for the \"version\" attribute to the root of the option XML" << endl;
               return false;
            }
         }
         if ( separator == xmlStrlen( prop ) - 1 ) {
            // Special case where the separator is the last character
            version *= 10;
         }
         else {
            i = separator + 1;
            if ( prop[i] >= '0' && prop[i] <= '9' ) {
               version *= 10;
               version += prop[i] - '0';
            }
            else {
               cerr << "Error: invalid format for the \"version\" attribute to the root of the option XML" << endl;
               return false;
            }
         }            
      }
      else {
         for ( i = 0; i < xmlStrlen( prop ); i++ ) {
            if ( prop[i] >= '0' && prop[i] <= '9' ) {
               version *= 10;
               version += prop[i] - '0';
            }
            else {
               cerr << "Error: invalid format for the \"version\" attribute to the root of the option XML" << endl;
               return false;
            }
         }
      }
   }
   xmlFree(prop);
   prop = NULL;

   if ( version < 20 ) {
      cerr << "This version of Error Parser requires version 2.0 or greater" << endl;
      cerr << "of the option file" << endl;
      return false;
   }
   
   node = root->children;

   //
   // General options
   //
   nodeValue = IsMandatoryValuePresent( node, "general_options" );
   if ( nodeValue == NULL ) {
      cerr << "Error: missing <general_options> in options file" << endl;
      return false;
   }
   if ( version == 20 ) {
      if ( ! GetGeneralOptions20( prefix, language, nodeValue->children ) ) {
         return false;
      }
   }
   else {
      if ( ! GetGeneralOptions( prefix, language, nodeValue->children, version, useTimestamp ) ) {
         return false;
      }
   }
   
   //
   // The standard header file for errors - optional section.
   //
   nodeValue = IsOptionalValuePresent( node, "header_file" );
   if ( nodeValue != NULL ) {
      if ( ! AddHeaderFileHandler( handlers, prefix, nodeValue->children, useTimestamp ) ) {
         return false;
      }
   }
   
   //
   // The header file and C code for error messages - optional section.
   //
   nodeValue = IsOptionalValuePresent( node, "errmsg_files" );
   if ( nodeValue != NULL ) {
      if ( version == 20 ) {
         if ( ! AddErrMessageHandlers20( handlers, prefix, nodeValue->children, useTimestamp ) ) {
            return false;
         }
      }
      else {
         if ( ! AddErrMessageHandlers( handlers, prefix, nodeValue->children, useTimestamp ) ) {
            return false;
         }
      }
   }

   //
   // C# code - optional section.
   //
   nodeValue = IsOptionalValuePresent( node, "csharp" );
   if ( nodeValue != NULL ) {
      if ( ! AddCSharpHandler( handlers, nodeValue->children, useTimestamp ) ) {
         return false;
      }
   }

   //
   // Code generator to add error codes to Python extension module - 
   // optional section.
   //
   nodeValue = IsOptionalValuePresent( node, "ext_python" );
   if ( nodeValue != NULL ) {
      if ( ! AddExtPython( handlers, nodeValue->children, useTimestamp ) ) {
         return false;
      }
   }

   //
   // Code generator to add error codes to Python - optional section.
   //
   nodeValue = IsOptionalValuePresent( node, "pure_python" );
   if ( nodeValue != NULL ) {
      if ( ! AddPurePythonHandler( handlers, nodeValue->children, useTimestamp ) ) {
         return false;
      }
   }

   //
   // Java code - optional section.
   //
   nodeValue = IsOptionalValuePresent( node, "java" );
   if ( nodeValue != NULL ) {
      
      if ( version >= 22 ) {
         javadocStyle = true;
         prop = xmlGetProp( nodeValue, BAD_CAST "javadoc_style" );
         if ( prop != NULL ) {
            if ( xmlStrcmp( prop, BAD_CAST "no") == 0 ) javadocStyle = false;
            xmlFree(prop);
         }
      }

      if ( ! AddJavaHandler( handlers, nodeValue->children, useTimestamp, javadocStyle ) ) {
         return false;
      }
   }
   
   xmlFreeDoc( doc );
   xmlFreeParserCtxt( context );
   xmlCleanupParser();

   return true;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

bool GetGeneralOptions( string  & prefix,
                        string  & language,
                        xmlNode * node,
                        int       version,
                        bool    & useTimestamp )
{
   xmlChar * value, * prop;
   string tmp, percent;
   xmlNode * nodeValue;
   size_t i;
   bool allowEscapes = false, allowQuotes = false;

   // The prefix to be used by the error code for files not using a namespace.
   // To avoid clashes with other libraries/software.
   value = GetMandatoryValue( node, "prefix_error_no_namespace" );
   if ( value == NULL ) {
      cerr << "Error: missing <prefix_error_no_namespace> in options file" << endl;
      return false;
   }
   stripText( value, tmp );
   if ( isAsciiStr(tmp.c_str(), tmp.length()) ) {
      for ( i = 0; i < tmp.length(); ++i ) {
         prefix += toupper(tmp[i]);
      }
   }
   else {
      cerr << "Error: <prefix_error_no_namespace> must be in ASCII" << endl;
      return false;
   }

   // Optional - select a language for error messages/documentation.
   nodeValue = IsOptionalValuePresent( node, "selected_lang" );
   if ( nodeValue != NULL ) {
      prop = xmlGetProp( nodeValue, BAD_CAST "lang" );
      if ( prop == NULL ) {
         cerr << "Error: missing \"xml:lang\" in options file" << endl;
         return false;
      }
      language = (char *)prop;
      xmlFree(prop);
   }

   nodeValue = IsMandatoryValuePresent( node, "message" );
   if ( nodeValue == NULL ) {
      cerr << "Error: missing <message> in options file" << endl;
      return false;
   }
   prop = xmlGetProp( nodeValue, BAD_CAST "allow_escapes" );
   if ( prop == NULL ) {
      cerr << "Error: missing \"allow_escapes\" in options file" << endl;
      return false;
   }
   if ( xmlStrcmp( prop, BAD_CAST "yes") == 0 ) allowEscapes = true;
   xmlFree(prop);
   
   prop = xmlGetProp( nodeValue, BAD_CAST "allow_quotes" );
   if ( prop == NULL ) {
      cerr << "Error: missing \"allow_quotes\" in options file" << endl;
      return false;
   }
   if ( xmlStrcmp( prop, BAD_CAST "yes") == 0 ) allowQuotes = true;
   xmlFree(prop);

   prop = xmlGetProp( nodeValue, BAD_CAST "percent" );
   if ( prop == NULL ) {
      cerr << "Error: missing \"percent\" in options file" << endl;
      return false;
   }
   percent = (char *)prop;
   xmlFree(prop);

   g_msgXML = new MessageXML( allowEscapes, allowQuotes, percent );

   if ( version >= 22 ) {
      // Optional - global code generation options.
      nodeValue = IsOptionalValuePresent( node, "code_generation" );
      if ( nodeValue != NULL ) {
         prop = xmlGetProp( nodeValue, BAD_CAST "timestamp" );
         if ( prop == NULL ) {
            cerr << "Error: missing \"timestamp\" in options file" << endl;
            return false;
         }
         if ( xmlStrcmp( prop, BAD_CAST "no") == 0 ) useTimestamp = false;
         xmlFree(prop);
      }
   }

   return true;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

bool GetGeneralOptions20( string  & prefix,
                          string  & language,
                          xmlNode * node )
{
   xmlChar * value, * prop;
   string tmp;
   xmlNode * nodeValue;
   size_t i;

   // The prefix to be used by the error code for files not using a namespace.
   // To avoid clashes with other libraries/software.
   value = GetMandatoryValue( node, "prefix_error_no_namespace" );
   if ( value == NULL ) {
      cerr << "Error: missing <prefix_error_no_namespace> in options file" << endl;
      return false;
   }
   stripText( value, tmp );
   if ( isAsciiStr(tmp.c_str(), tmp.length()) ) {
      for ( i = 0; i < tmp.length(); ++i ) {
         prefix += toupper(tmp[i]);
      }
   }
   else {
      cerr << "Error: <prefix_error_no_namespace> must be in ASCII" << endl;
      return false;
   }

   // Optional - select a language for error messages/documentation.
   nodeValue = IsOptionalValuePresent( node, "selected_lang" );
   if ( nodeValue != NULL ) {
      prop = xmlGetProp( nodeValue, BAD_CAST "lang" );
      if ( prop == NULL ) {
         cerr << "Error: missing \"xml:lang\" in options file" << endl;
         return false;
      }
      language = (char *)prop;
      xmlFree(prop);
   }

   return true;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

bool AddHeaderFileHandler( vector<AbstractHandler *> & handlers,
                           string                    & prefix,
                           xmlNode                   * node,
                           bool                        useTimestamp )
{
   bool usingEnums = false;
   string enumname, dirname, filename;
   xmlChar * value;
   ErrorHeader * p;
   
   // enum information is optional - it must be present if the target is an 
   // enum. If not present, we use "#define" instead.
   value = GetOptionalValue( node, "header_enum" );
   if ( value != NULL ) {
      usingEnums = true;
      stripText( value, enumname );
   }

   // The directory of the output header file for the error codes.
   value = GetOptionalValue( node, "header_dirname" );
   if ( value != NULL ) {
      stripText( value, dirname );
   }
   
   // The name of the output header file for the error codes.
   value = GetMandatoryValue( node, "header_name" );
   if ( value != NULL ) {
      stripText( value, filename );
   }
   else {
      cerr << "Error: missing <header_name> in options file" << endl;
      return false;
   }
   
   // Create the header file handler 
   try {
      p = new ErrorHeader( useTimestamp, dirname, filename, enumname, prefix );
   }
   catch (exception& e) {
      cerr << "Error Header handler exception caught opening the output file " << endl;
      throw e;
   }
   
   handlers.push_back(p);
   
   return true;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

bool AddErrMessageHandlers( vector<AbstractHandler *> & handlers,
                            string                    & prefix,
                            xmlNode                   * node,
                            bool                        useTimestamp )
{
   string enumname, dirname, headername, cname, varPrefix;
   xmlChar * value, * prop;
   xmlNode * valueNode;
   bool buildDll = false;
   ErrMessageHeader * pH;
   ErrMessage * pC;
   
   value = GetOptionalValue( node, "errmsg_dirname" );
   if ( value != NULL ) {
      stripText( value, dirname );
   }

   value = GetMandatoryValue( node, "errmsg_header_name" );
   if ( value == NULL ) {
      cerr << "Error: missing <errmsg_header_name> in options file" << endl;
      return false;
   }
   stripText( value, headername );

   value = GetMandatoryValue( node, "errmsg_c_fullname" );
   if ( value == NULL ) {
      cerr << "Error: missing <errmsg_c_fullname> in options file" << endl;
      return false;
   }
   stripText( value, cname );

   valueNode = IsMandatoryValuePresent( node, "errmsg_options" );
   if ( valueNode == NULL ) {
      cerr << "Error: missing <errmsg_c_fullname> in options file" << endl;
      return false;
   }
   prop = xmlGetProp( valueNode, BAD_CAST "build_dll" );
   if ( prop == NULL ) {
      cerr << "Error: missing \"build_dll\" in options file" << endl;
      return false;
   }
   if ( xmlStrcmp( prop, BAD_CAST "yes") == 0 ) buildDll = true;
   xmlFree(prop);

   value = GetMandatoryValue( node, "errmsg_prefix_var" );
   if ( value == NULL ) {
      cerr << "Error: missing <errmsg_prefix_var> in options file" << endl;
      return false;
   }
   stripText( value, varPrefix );

   try {
      pH = new ErrMessageHeader( useTimestamp, dirname, headername, varPrefix, buildDll );
   }
   catch (exception& e) {
      cerr << "Err Msg handler (.h) exception caught opening the output file " << endl;
      throw e;
   }
   
   try {
      pC = new ErrMessage( useTimestamp, cname, headername, prefix, varPrefix );
   }
   catch (exception& e) {
      cerr << "Err Msg handler (.c) exception caught opening the output file " << endl;
      throw e;
   }

   handlers.push_back(pH);
   handlers.push_back(pC);

   return true;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

bool AddErrMessageHandlers20( vector<AbstractHandler *> & handlers,
                              string                    & prefix,
                              xmlNode                   * node,
                              bool                        useTimestamp )
{
   string enumname, dirname, headername, cname, varPrefix, percent;
   xmlChar * value, * prop;
   xmlNode * valueNode;
   bool buildDll = false, allowEscapes = false, allowQuotes = false;
   ErrMessageHeader * pH;
   ErrMessage * pC;
   
   value = GetOptionalValue( node, "errmsg_dirname" );
   if ( value != NULL ) {
      stripText( value, dirname );
   }

   value = GetMandatoryValue( node, "errmsg_header_name" );
   if ( value == NULL ) {
      cerr << "Error: missing <errmsg_header_name> in options file" << endl;
      return false;
   }
   stripText( value, headername );

   value = GetMandatoryValue( node, "errmsg_c_fullname" );
   if ( value == NULL ) {
      cerr << "Error: missing <errmsg_c_fullname> in options file" << endl;
      return false;
   }
   stripText( value, cname );

   valueNode = IsMandatoryValuePresent( node, "errmsg_options" );
   if ( valueNode == NULL ) {
      cerr << "Error: missing <errmsg_c_fullname> in options file" << endl;
      return false;
   }
   prop = xmlGetProp( valueNode, BAD_CAST "build_dll" );
   if ( prop == NULL ) {
      cerr << "Error: missing \"build_dll\" in options file" << endl;
      return false;
   }
   if ( xmlStrcmp( prop, BAD_CAST "yes") == 0 ) buildDll = true;
   xmlFree(prop);

   value = GetMandatoryValue( node, "errmsg_prefix_var" );
   if ( value == NULL ) {
      cerr << "Error: missing <errmsg_prefix_var> in options file" << endl;
      return false;
   }
   stripText( value, varPrefix );

   valueNode = IsMandatoryValuePresent( node, "errmsg_msg" );
   if ( valueNode == NULL ) {
      cerr << "Error: missing <errmsg_msg> in options file" << endl;
      return false;
   }
   prop = xmlGetProp( valueNode, BAD_CAST "allow_escapes" );
   if ( prop == NULL ) {
      cerr << "Error: missing \"allow_escapes\" in options file" << endl;
      return false;
   }
   if ( xmlStrcmp( prop, BAD_CAST "yes") == 0 ) allowEscapes = true;
   xmlFree(prop);
   
   prop = xmlGetProp( valueNode, BAD_CAST "allow_quotes" );
   if ( prop == NULL ) {
      cerr << "Error: missing \"allow_quotes\" in options file" << endl;
      return false;
   }
   if ( xmlStrcmp( prop, BAD_CAST "yes") == 0 ) allowQuotes = true;
   xmlFree(prop);

   prop = xmlGetProp( valueNode, BAD_CAST "percent" );
   if ( prop == NULL ) {
      cerr << "Error: missing \"percent\" in options file" << endl;
      return false;
   }
   percent = (char *)prop;
   xmlFree(prop);

   try {
      pH = new ErrMessageHeader( useTimestamp, dirname, headername, varPrefix, buildDll );
   }
   catch (exception& e) {
      cerr << "Err Msg handler (.h) exception caught opening the output file " << endl;
      throw e;
   }
   
   try {
      pC = new ErrMessage( useTimestamp, cname, headername, prefix, varPrefix );
   }
   catch (exception& e) {
      cerr << "Err Msg handler (.c) exception caught opening the output file " << endl;
      throw e;
   }
   g_msgXML = new MessageXML( allowEscapes, allowQuotes, percent );
   
   handlers.push_back(pH);
   handlers.push_back(pC);

   return true;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

bool AddCSharpHandler( vector<AbstractHandler *> & handlers,
                       xmlNode                   * node,
                       bool                        useTimestamp )
{
   string filename, enumname, cs_namespace;
   xmlChar * value;
   CSharp * p;
   
   value = GetMandatoryValue( node, "cs_filename" );
   if ( value == NULL ) {
      cerr << "Error: missing <cs_filename> in options file" << endl;
      return false;
   }
   stripText( value, filename );

   value = GetMandatoryValue( node, "cs_enum_name" );
   if ( value == NULL ) {
      cerr << "Error: missing <cs_enum_name> in options file" << endl;
      return false;
   }
   stripText( value, enumname );

   value = GetOptionalValue( node, "cs_namespace" );
   if ( value != NULL ) {
      stripText( value, cs_namespace );
   }

   try {
      p = new CSharp( useTimestamp, filename, cs_namespace, enumname );
   }
   catch (exception& e) {
      cerr << "C# handler exception caught opening the output file " << endl;
      throw e;
   }
   
   handlers.push_back(p);

   return true;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

bool AddExtPython( vector<AbstractHandler *> & handlers,
                   xmlNode                   * node,
                   bool                        useTimestamp )
{
   string dirname, filename, functionName;
   xmlChar * value;
   ExtPython * p;

   value = GetOptionalValue( node, "ext_py_dirname" );
   if ( value != NULL ) {
      stripText( value, dirname );
   }

   value = GetMandatoryValue( node, "ext_py_filename" );
   if ( value == NULL ) {
      cerr << "Error: missing <ext_py_filename> in options file" << endl;
      return false;
   }
   stripText( value, filename );

   value = GetOptionalValue( node, "ext_py_function" );
   if ( value != NULL ) {
      stripText( value, functionName );
   }

   try {
      p = new ExtPython( useTimestamp, dirname, filename, functionName );
   }
   catch (exception& e) {
      cerr << "Python ext. handler exception caught opening the output file " << endl;
      throw e;
   }
   
   handlers.push_back(p);

   return true;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

bool AddPurePythonHandler( vector<AbstractHandler *> & handlers,
                           xmlNode                   * node,
                           bool                        useTimestamp )
{
   string filename, functionName;
   xmlChar * value;
   PurePython * p;
   
   value = GetMandatoryValue( node, "pure_py_filename" );
   if ( value == NULL ) {
      cerr << "Error: missing <pure_py_filename> in options file" << endl;
      return false;
   }
   stripText( value, filename );

   value = GetOptionalValue( node, "pure_py_function" );
   if ( value != NULL ) {
      stripText( value, functionName );
   }

   try {
      p = new PurePython( useTimestamp, filename, functionName );
   }
   catch (exception& e) {
      cerr << "Python handler exception caught opening the output file " << endl;
      throw e;
   }
   
   handlers.push_back(p);

   return true;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

bool AddJavaHandler( vector<AbstractHandler *> & handlers,
                     xmlNode                   * node,
                     bool                        useTimestamp,
                     bool                        javadocStyle )
{
   string filename, enumname, java_package;
   xmlChar * value;
   Java * p;
   
   value = GetMandatoryValue( node, "java_filename" );
   if ( value == NULL ) {
      cerr << "Error: missing <java_filename> in options file" << endl;
      return false;
   }
   stripText( value, filename );

   value = GetMandatoryValue( node, "java_enum_name" );
   if ( value == NULL ) {
      cerr << "Error: missing <java_enum_name> in options file" << endl;
      return false;
   }
   stripText( value, enumname );

   value = GetOptionalValue( node, "java_package" );
   if ( value != NULL ) {
      stripText( value, java_package );
   }

   try {
      p = new Java( useTimestamp, filename, java_package, enumname, javadocStyle );
   }
   catch (exception& e) {
      cerr << "Java handler exception caught opening the output file " << endl;
      throw e;
   }
   
   handlers.push_back(p);

   return true;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

