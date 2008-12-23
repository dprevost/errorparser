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

#include "parser1.h"
#include "AbstractHandler.h"
#include "ErrorHeader.h"

using namespace std;

bool GetHeaderFile( vector<AbstractHandler> & handlers,
                    xmlNode                 * child );

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

int handleOptions( vector<AbstractHandler> & handlers,
                   errp_common             * commonArgs, 
                   int                       argc, 
                   char                    * argv[] )
{
   xmlParserCtxtPtr context = NULL;  /* The parser context */
   xmlNode * root = NULL, * node, * child = NULL, * nodeValue;
   xmlDoc  * doc;
   int i;
   xmlChar * prop, * value;
   int version;
   string tmp;
   
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
   commonArgs->xmlFileName = argv[argc-1];

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
   if ( version < 20 ) {
      cerr << "This version of Error Parser requires version 2.0 or greater" << endl;
      cerr << "of the option file" << endl;
   }
   xmlFree(prop);
   prop = NULL;
   
   node = root->children;

   //
   // General options
   //
   
   // The prefix to be used by the error code for files not using a namespace.
   // To avoid clashes with other libraries/software.
   value = GetMandatoryValue( node, "prefix_error_no_namespace" );
   if ( value == NULL ) {
      cerr << "Error: missing <prefix_error_no_namespace> in options file" << endl;
      return -1;
   }
   stripText( value, tmp );
   if ( isAsciiStr(tmp.c_str(), tmp.length()) ) {
      for ( i = 0; i < (int)tmp.length(); ++i ) {
         commonArgs->prefix += toupper(tmp[i]);
      }
   }
   else {
      cerr << "Error: <prefix_error> must be in ASCII" << endl;
      return -1;
   }

   // Optional - selec a language for error messages/documentation.
   nodeValue = IsOptionalValuePresent( node, "selected_lang" );
   if ( nodeValue != NULL ) {
      prop = xmlGetProp( nodeValue, BAD_CAST "lang" );
      if ( prop == NULL ) {
         cerr << "Error: missing \"xml:lang\" in options file" << endl;
         return -1;
      }
      commonArgs->language = prop;
   }
   
   //
   // The standard header file for errors - optional section.
   //
   nodeValue = IsOptionalValuePresent( node, "header_file" );
   if ( nodeValue != NULL ) {
      if ( ! GetHeaderFile( handlers, nodeValue->children ) ) {
         return -1;
      }
   }
   
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "csharp") == 0 ) {
            commonArgs->using_cs = 1;
               child = node->children;
               node = node->next;
            }
            /*
             * No node->next if the if is false. The current node might be 
             * used, eventually, for Java or Python or ...
             */
            break;
         }
         node = node->next;
      }

   

   /* The directory of the output files for the error messages. */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "errmsg_dir") == 0 ) {
            stripText( node->children->content, commonArgs->outputDir );
            node = node->next;
         }
         break;
      }
      node = node->next;
   }

   /* The name of the output header file for the error messages. */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "errmsg_header_name") == 0 ) {
            stripText( node->children->content, commonArgs->outputNameH );
            node = node->next;
            break;
         }
         cerr << "Error: missing <errmsg_header_name> in options file" << endl;
         return -1;
      }
      node = node->next;
   }

   /* The name of the output C file for the error messages. */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "errmsg_c_name") == 0 ) {
            stripText( node->children->content, commonArgs->outputNameC );
            node = node->next;
            break;
         }
         cerr << "Error: missing <errmsg_c_name> in options file" << endl;
         return -1;
      }
      node = node->next;
   }

   /* Do we need to export the error message function? */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "errmsg_options") == 0 ) {
            prop = xmlGetProp( node, BAD_CAST "build_dll" );
            if ( prop == NULL ) {
               cerr << "Error: missing \"build_dll\" in options file" << endl;
               return -1;
            }
            commonArgs->build_dll = 0;
            if ( xmlStrcmp( prop, BAD_CAST "yes") == 0 ) {
               commonArgs->build_dll = 1;
            }
            xmlFree(prop);
            node = node->next;
         }
         break;
      }
      node = node->next;
   }


   /* The prefix to be used for the variables of the C code for error messages. */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "prefix_variable") == 0 ) {
            stripText( node->children->content, commonArgs->varPrefix );
            node = node->next;
            break;
         }
         cerr << "Error: missing <prefix_variable> in options file" << endl;
         return -1;
      }
      node = node->next;
   }

   /* The attributes of the errmsg. */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "err_msg") == 0 ) {
            prop = xmlGetProp( node, BAD_CAST "allow_escapes" );
            if ( prop == NULL ) {
               cerr << "Error: missing \"allow_escapes\" in options file" << endl;
               return -1;
            }
            commonArgs->allowEscapes = 0;
            if ( xmlStrcmp( prop, BAD_CAST "yes") == 0 ) {
               commonArgs->allowEscapes = 1;
            }
            xmlFree(prop);

            prop = xmlGetProp( node, BAD_CAST "allow_quotes" );
            if ( prop == NULL ) {
               cerr << "Error: missing \"allow_quotes\" in options file" << endl;
               return -1;
            }
            commonArgs->allowQuotes = 0;
            if ( xmlStrcmp( prop, BAD_CAST "yes") == 0 ) {
               commonArgs->allowQuotes = 1;
            }
            xmlFree(prop);
            
            prop = xmlGetProp( node, BAD_CAST "percent" );
            if ( prop == NULL ) {
               cerr << "Error: missing \"percent\" in options file" << endl;
               return -1;
            }
            commonArgs->percent = prop;

            node = node->next;
            break;
         }
         cerr << "Error: missing <err_msg> in options file" << endl;
         return -1;
      }
      node = node->next;
   }

   /* This is the last element of the option file (v 1.0) - it is optional */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "selected_lang") == 0 ) {
            prop = xmlGetProp( node, BAD_CAST "lang" );
            if ( prop == NULL ) {
               cerr << "Error: missing \"xml:lang\" in options file" << endl;
               return -1;
            }
            commonArgs->language = prop;
            node = node->next;
         }
         break; 
      }
      node = node->next;
   }
  
   commonArgs->using_cs = 0;
   if ( version > 10 ) {
      /* Will we create a C# enum? */
      while ( node != NULL ) {
         if ( node->type == XML_ELEMENT_NODE ) {
            if ( xmlStrcmp( node->name, BAD_CAST "csharp") == 0 ) {
               commonArgs->using_cs = 1;
               child = node->children;
               node = node->next;
            }
            /*
             * No node->next if the if is false. The current node might be 
             * used, eventually, for Java or Python or ...
             */
            break;
         }
         node = node->next;
      }

      if ( commonArgs->using_cs ) {
         /* The name of the output file for the C# enum. */
         while ( child != NULL ) {
            if ( child->type == XML_ELEMENT_NODE ) {
               if ( xmlStrcmp( child->name, BAD_CAST "cs_filename") == 0 ) {
                  stripText( child->children->content, commonArgs->cs_filename );
                  child = child->next;
                  break;
               }
               cerr << "Error: missing <cs_filename> in options file" << endl;
               return -1;
            }
            child = child->next;
         }
         /* The name of the enum for the C# enum. */
         while ( child != NULL ) {
            if ( child->type == XML_ELEMENT_NODE ) {
               if ( xmlStrcmp( child->name, BAD_CAST "cs_enum") == 0 ) {
                  stripText( child->children->content, commonArgs->cs_enum );
                  child = child->next;
                  break;
               }
               cerr << "Error: missing <cs_enum> in options file" << endl;
               return -1;
            }
            child = child->next;
         }
         /* The name of the C# namespace. */
         while ( child != NULL ) {
            if ( child->type == XML_ELEMENT_NODE ) {
               if ( xmlStrcmp( child->name, BAD_CAST "cs_namespace") == 0 ) {
                  stripText( child->children->content, commonArgs->cs_namespace );
                  child = child->next;
               }
               break;
            }
            child = child->next;
         }

      }
   }

   commonArgs->using_py = 0;
   if ( version > 11 ) {
      /* Will we create a python dictionary? */
      while ( node != NULL ) {
         if ( node->type == XML_ELEMENT_NODE ) {
            if ( xmlStrcmp( node->name, BAD_CAST "python") == 0 ) {
               commonArgs->using_py = 1;
               child = node->children;
               node = node->next;
            }
            /*
             * No node->next if the if is false. The current node might be 
             * used, eventually, for Java or ...
             */
            break;
         }
         node = node->next;
      }

      if ( commonArgs->using_py ) {
         /* Using extended module (or pure python)? */
         while ( child != NULL ) {
            if ( child->type == XML_ELEMENT_NODE ) {
               if ( xmlStrcmp( child->name, BAD_CAST "py_options") == 0 ) {
                  prop = xmlGetProp( child, BAD_CAST "extended" );
                  if ( prop == NULL ) {
                     cerr << "Error: missing   \"py_options:extended\" in options file" << endl;
                     return -1;
                  }
                  commonArgs->using_py_extended = 0;
                  if ( xmlStrcmp( prop, BAD_CAST "yes") == 0 ) {
                     commonArgs->using_py_extended = 1;
                  }
                  xmlFree(prop);
                  child = child->next;
                  break;
               }
               cerr << "Error: missing <py_options> in options file" << endl;
               return -1;
            }
            child = child->next;
         }
         /* The directory name for the python output file (optional). */
         while ( child != NULL ) {
            if ( child->type == XML_ELEMENT_NODE ) {
               if ( xmlStrcmp( child->name, BAD_CAST "py_dirname") == 0 ) {
                  stripText( child->children->content, commonArgs->py_dirname );
                  child = child->next;
               }
               break;
            }
            child = child->next;
         }
         /* The name of the python output file (C header file or pure python). */
         while ( child != NULL ) {
            if ( child->type == XML_ELEMENT_NODE ) {
               if ( xmlStrcmp( child->name, BAD_CAST "py_filename") == 0 ) {
                  stripText( child->children->content, commonArgs->py_filename );
                  child = child->next;
                  break;
               }
               cerr << "Error: missing <py_filename> in options file" << endl;
               return -1;
            }
            child = child->next;
         }
      }
   }

   xmlFreeDoc( doc );
   xmlFreeParserCtxt( context );
   xmlCleanupParser();

   return 0;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

bool GetHeaderFile( vector<AbstractHandler> & handlers,
                    xmlNode                 * node )
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
      stripText( node->children->content, enumname );
   }

   /* The directory of the output header file for the error codes. */
   value = GetOptionalValue( node, "header_dirname" );
   if ( value != NULL ) {
      stripText( node->children->content, dirname );
   }
   
   /* The name of the output header file for the error codes. */
   value = GetMandatoryValue( node, "header_name" );
   if ( value != NULL ) {
      stripText( node->children->content, filename );
   }
   else {
      cerr << "Error: missing <header_name> in options file" << endl;
      return false;
   }
   
   // Create the header file handler 
   p = new ErrorHeader();
   
   // Add our handler to 
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

