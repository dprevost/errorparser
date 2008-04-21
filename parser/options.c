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

#include "parser.h"

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void usage( char * progName ) 
{
   fprintf( stderr, "Usage:\n\n %s %s %s %s %s\n",
      progName,
      "-e|--error error_header_file",
      "-o|--output prefix_for_output_c_file",
      "-v|--varprefix prefix_for_variable",
      "input_xml_file" );
   fprintf( stderr, " or\n" );
   fprintf( stderr, "%s -h|-?|--help\n\n", progName );
   fprintf( stderr, "Options:\n\n" );
   fprintf( stderr, "  -e,--error     the name of the header file that the program will create.\n" );
   fprintf( stderr, "  -o,--output    the prefix for the name of the header and C files that the \n" );
   fprintf( stderr, "                 program will generate to retrieve the error messages.\n" );
   fprintf( stderr, "  -v,--varprefix the prefix for the names of the variables to be used in the \n" );
   fprintf( stderr, "                 header and C files (used to retrieve the error messages.\n\n" );

}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

int handleOptions( errp_common * commonArgs, int argc, char * argv[] )
{
   xmlParserCtxtPtr context = NULL;  /* The parser context */
   xmlNode * root = NULL, * node;
   xmlDoc  * doc;
   int i;
   xmlChar * prop;
   
   if ( argc == 2 ) {
      if ( strcmp("--help",argv[1]) == 0 || strcmp("-h",argv[1]) == 0 ||
           strcmp("-?",argv[1]) == 0 ) {
         usage( argv[0] );
         return 1;
      }
   }
   if (argc != 3) {
      usage( argv[0] );
      return -1;
   }
   
   if ( (strcmp("--options",argv[1]) != 0) && (strcmp("-o",argv[1]) != 0) ) {
      usage( argv[0] );
      return -1;
   }
//   commonArgs->xmlFileName = argv[argc-1];

   context = xmlNewParserCtxt();
   if ( context == NULL ) {
      fprintf(stderr, "Error allocating the parser context\n");
      return -1;
   }

   /* We create the document and validate in one go */
   doc = xmlCtxtReadFile( context, 
                          argv[2],
                          NULL,
                          XML_PARSE_DTDVALID );
   if ( doc == NULL ) {
      fprintf( stderr, "Error while parsing the option file: %s\n", argv[2] );
      return -1;
   }
   if ( context->valid == 0 ) {
      fprintf( stderr, "Error: document validation (for options) failed\n" );
      return -1;
   }
   
   root = xmlDocGetRootElement( doc );

   node = root->children;
   
   /*
    * Get the xml filename.
    *
    * Note: from this point on we must check for NULL nodes because the
    * DTD is not an external document.
    */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "xmlname") == 0 ) {
            commonArgs->xmlFileName = stripText( node->children->content ); 
            node = node->next;
            break;
         }
         fprintf( stderr, "Error: missing <xmlname> in options file\n" );
         return -1;
      }
      node = node->next;
   }

   /* 
    * enum information is only present if the target is an enum. If not
    * present, we use "#define" instead.
    */
   commonArgs->writingEnum = 0;
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "enumname") == 0 ) {
            commonArgs->writingEnum = 1;
            commonArgs->enumname = stripText( node->children->content );
            node = node->next;
         }
         break;
      }
      node = node->next;
   }

   /* The directory of the output header file for the error codes. */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "header_name_dir") == 0 ) {
            commonArgs->headerDir = stripText( node->children->content );
            node = node->next;
         }
         break;
      }
      node = node->next;
   }

   /* The name of the output header file for the error codes. */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "header_name") == 0 ) {
            commonArgs->headerName = stripText( node->children->content );
            node = node->next;
            break;
         }
         fprintf( stderr, "Error: missing <header_name> in options file\n" );
         return -1;
      }
      node = node->next;
   }

   /* The directory of the output files for the error messages. */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "errmsg_dir") == 0 ) {
            commonArgs->outputDir = stripText( node->children->content );
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
            commonArgs->outputNameH = stripText( node->children->content );
            node = node->next;
            break;
         }
         fprintf( stderr, "Error: missing <errmsg_header_name> in options file\n" );
         return -1;
      }
      node = node->next;
   }

   /* The name of the output C file for the error messages. */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "errmsg_c_name") == 0 ) {
            commonArgs->outputNameC = stripText( node->children->content );
            node = node->next;
            break;
         }
         fprintf( stderr, "Error: missing <errmsg_c_name> in options file\n" );
         return -1;
      }
      node = node->next;
   }

   /* The prefix to be used by the error code. */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "prefix_error") == 0 ) {
            commonArgs->prefix = stripText( node->children->content );
            if ( isAsciiStr(commonArgs->prefix) ) {
               for ( i = 0; i < xmlStrlen(commonArgs->prefix); ++i ) {
                  commonArgs->prefix[i] = toupper(commonArgs->prefix[i]);
               }
            }
            else {
               fprintf( stderr, "Error: <prefix_error> must be in ASCII\n" );
               return -1;
            }
            node = node->next;
            break;
         }
         fprintf( stderr, "Error: missing <prefix_error> in options file\n" );
         return -1;
      }
      node = node->next;
   }

   /* The prefix to be used for the variables of the C code for error messages. */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "prefix_variable") == 0 ) {
            commonArgs->varPrefix = stripText( node->children->content );
            node = node->next;
            break;
         }
         fprintf( stderr, "Error: missing <prefix_variable> in options file\n" );
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
               fprintf( stderr, "Error: missing \"allow_escapes\" in options file\n" );
               return -1;
            }
            commonArgs->allowEscapes = 0;
            if ( xmlStrcmp( prop, BAD_CAST "yes") == 0 ) {
               commonArgs->allowEscapes = 1;
            }

            prop = xmlGetProp( node, BAD_CAST "allow_quotes" );
            if ( prop == NULL ) {
               fprintf( stderr, "Error: missing \"allow_quotes\" in options file\n" );
               return -1;
            }
            commonArgs->allowQuotes = 0;
            if ( xmlStrcmp( prop, BAD_CAST "yes") == 0 ) {
               commonArgs->allowQuotes = 1;
            }
            
            prop = xmlGetProp( node, BAD_CAST "percent" );
            if ( prop == NULL ) {
               fprintf( stderr, "Error: missing \"percent\" in options file\n" );
               return -1;
            }
            commonArgs->percent = prop;

            break;
         }
         fprintf( stderr, "Error: missing <err_msg> in options file\n" );
         return -1;
      }
      node = node->next;
   }

   xmlFreeDoc( doc );
   xmlFreeParserCtxt( context );
   xmlCleanupParser();

   return 0;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

