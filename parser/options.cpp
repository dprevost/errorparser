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

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

#include <vector>
#include <iostream>

#include "parser.h"
#include "AbstractHandler.h"
#include "ErrorHeader.h"
#include "ErrMessageHeader.h"
#include "ErrMessage.h"
#include "CSharp.h"
#include "ExtPython.h"
#include "PurePython.h"

using namespace std;

bool GetGeneralOptions( string  & prefix,
                        string  & language,
                        xmlNode * node );

bool AddHeaderFileHandler( vector<AbstractHandler *> & handlers,
                           string                    & prefix,
                           xmlNode                   * child );

bool AddErrMessageHandlers( vector<AbstractHandler *> & handlers,
                            string                    & prefix,
                            xmlNode                   * node );

bool AddCSharpHandler( vector<AbstractHandler *> & handlers,
                       xmlNode                   * node );

bool AddExtPython( vector<AbstractHandler *> & handlers,
                   xmlNode                   * node );

bool AddPurePythonHandler( vector<AbstractHandler *> & handlers,
                           xmlNode                   * node );

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void usage( char * progName ) 
{
   cerr << "Usage:" << endl << endl;
   cerr << progName << "-o|--options options_xml_file  input_xml_file" << endl;
   cerr << "  or" << endl;
   cerr << progName << " -h|-?|--help" << endl << endl;
   cerr << "Options:" << endl << endl;
   cerr << "  -o,--options   the name of the xml file defining the options that will be" << endl;
   cerr << "                 used to generate the code." << endl;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

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

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

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

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

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

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

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

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

int handleOptions( vector<AbstractHandler *> & handlers,
                   string                    & xmlFileName,
                   string                    & language,
                   int                         argc, 
                   char                      * argv[] )
{
   xmlParserCtxtPtr context = NULL;  /* The parser context */
   xmlNode * root = NULL, * node, * nodeValue;
   xmlDoc  * doc;
   int i;
   xmlChar * prop;
   int version;
   string tmp;
   string prefix;
   
   if ( argc == 2 ) {
      if ( strcmp("--help",argv[1]) == 0 || strcmp("-h",argv[1]) == 0 ||
           strcmp("-?",argv[1]) == 0 ) {
         usage( argv[0] );
         return 1;
      }
   }
   if (argc != 4) {
      usage( argv[0] );
      return -1;
   }
   
   if ( (strcmp("--options",argv[1]) != 0) && (strcmp("-o",argv[1]) != 0) ) {
      usage( argv[0] );
      return -1;
   }
   xmlFileName = argv[argc-1];

   context = xmlNewParserCtxt();
   if ( context == NULL ) {
      cerr << "Error allocating the parser context" << endl;
      return -1;
   }

   /* We create the document and validate in one go */
   doc = xmlCtxtReadFile( context, 
                          argv[2],
                          NULL,
                          XML_PARSE_DTDVALID );
   if ( doc == NULL ) {
      cerr << "Error while parsing the option file: " << argv[2] << endl;
      return -1;
   }
   if ( context->valid == 0 ) {
      cerr << "Error: document validation (for options) failed" << endl;
      return -1;
   }
   
   root = xmlDocGetRootElement( doc );

   prop = xmlGetProp( root, BAD_CAST "version" );
   if ( prop == NULL ) {
      cerr << "Error: missing \"version\" attribute to the root of the option XML" << endl;
      return -1;
   }
   else {
      for ( i = 0, version = 0; i < xmlStrlen( prop ); i++ ) {
         if ( prop[i] >= '0' && prop[i] <= '9' ) {
            version *= 10;
            version += prop[i] - '0';
         }
      }
   }
   xmlFree(prop);
   prop = NULL;

   if ( version < 20 ) {
      cerr << "This version of Error Parser requires version 2.0 or greater" << endl;
      cerr << "of the option file" << endl;
      return -1;
   }
   
   node = root->children;

   //
   // General options
   //
   nodeValue = IsMandatoryValuePresent( node, "general_options" );
   if ( nodeValue == NULL ) {
      cerr << "Error: missing <general_options> in options file" << endl;
      return -1;
   }
   if ( ! GetGeneralOptions( prefix, language, nodeValue->children ) ) {
      return -1;
   }

   //
   // The standard header file for errors - optional section.
   //
   nodeValue = IsOptionalValuePresent( node, "header_file" );
   if ( nodeValue != NULL ) {
      if ( ! AddHeaderFileHandler( handlers, prefix, nodeValue->children ) ) {
         return -1;
      }
   }
   
   //
   // The header file and C code for error messages - optional section.
   //
   nodeValue = IsOptionalValuePresent( node, "errmsg_files" );
   if ( nodeValue != NULL ) {
      if ( ! AddErrMessageHandlers( handlers, prefix, nodeValue->children ) ) {
         return -1;
      }
   }

   //
   // C# code - optional section.
   //
   nodeValue = IsOptionalValuePresent( node, "csharp" );
   if ( nodeValue != NULL ) {
      if ( ! AddCSharpHandler( handlers, nodeValue->children ) ) {
         return -1;
      }
   }

   //
   // Code generator to add error codes to Python extension module - 
   // optional section.
   //
   nodeValue = IsOptionalValuePresent( node, "ext_python" );
   if ( nodeValue != NULL ) {
      if ( ! AddExtPython( handlers, nodeValue->children ) ) {
         return -1;
      }
   }

   //
   // Code generator to add error codes to Python - optional section.
   //
   nodeValue = IsOptionalValuePresent( node, "pure_python" );
   if ( nodeValue != NULL ) {
      if ( ! AddPurePythonHandler( handlers, nodeValue->children ) ) {
         return -1;
      }
   }

   xmlFreeDoc( doc );
   xmlFreeParserCtxt( context );
   xmlCleanupParser();

   return 0;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

bool GetGeneralOptions( string  & prefix,
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

   // Optional - selec a language for error messages/documentation.
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

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

bool AddHeaderFileHandler( vector<AbstractHandler *> & handlers,
                           string                    & prefix,
                           xmlNode                   * node )
{
   bool usingEnums = false;
   string enumname, dirname, filename;
   xmlChar * value;
   ErrorHeader * p;
   
   /* 
    * enum information is only present if the target is an enum. If not
    * present, we use "#define" instead.
    */
   value = GetOptionalValue( node, "header_enum" );
   if ( value != NULL ) {
      usingEnums = true;
      stripText( value, enumname );
   }

   /* The directory of the output header file for the error codes. */
   value = GetOptionalValue( node, "header_dirname" );
   if ( value != NULL ) {
      stripText( value, dirname );
   }
   
   /* The name of the output header file for the error codes. */
   value = GetMandatoryValue( node, "header_name" );
   if ( value != NULL ) {
      stripText( value, filename );
   }
   else {
      cerr << "Error: missing <header_name> in options file" << endl;
      return false;
   }
   
   // Create the header file handler 
   p = new ErrorHeader( dirname, filename, enumname, prefix );
   
   handlers.push_back(p);
   
   return true;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

bool AddErrMessageHandlers( vector<AbstractHandler *> & handlers,
                            string                    & prefix,
                            xmlNode                   * node )
{
   string enumname, dirname, headername, cname, varPrefix, percent;
   xmlChar * value, * prop;
   xmlNode * valueNode;
   bool buildDll = false, allowEscapes = false, allowQuotes = false;
   ErrMessageHeader * pH;
   ErrMessage * pC;
   
   value = GetOptionalValue( node, "errmsg_dir" );
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

   pH = new ErrMessageHeader( dirname, headername, varPrefix, buildDll );
   pC = new ErrMessage( cname, headername, prefix, varPrefix, 
      allowEscapes, allowQuotes );
   
   handlers.push_back(pH);
   handlers.push_back(pC);

   return true;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

bool AddCSharpHandler( vector<AbstractHandler *> & handlers,
                       xmlNode                   * node )
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

   p = new CSharp( filename, cs_namespace, enumname );
   
   handlers.push_back(p);

   return true;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

bool AddExtPython( vector<AbstractHandler *> & handlers,
                   xmlNode                   * node )
{
   string dirname, filename;
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

   p = new ExtPython( dirname, filename );
   
   handlers.push_back(p);

   return true;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

bool AddPurePythonHandler( vector<AbstractHandler *> & handlers,
                           xmlNode                   * node )
{
   string filename;
   xmlChar * value;
   PurePython * p;
   
   value = GetMandatoryValue( node, "pure_py_filename" );
   if ( value == NULL ) {
      cerr << "Error: missing <pure_py_filename> in options file" << endl;
      return false;
   }
   stripText( value, filename );

   p = new PurePython( filename );
   
   handlers.push_back(p);

   return true;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

