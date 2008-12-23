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

using namespace std;

void addGroup( errp_common * commonArgs, xmlNode * group, int last );
int handleOptions( vector<AbstractHandler> & handlers, 
                   errp_common             * commonArgs, 
                   int                       argc, 
                   char                    * argv[] );

#if 0

const char * g_barrier = "/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */";
const char * g_barrierPy = "# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--";
const char * g_functionName = "_ErrorMessage";

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void doTopOfFile( errp_common * commonArgs,
                  xmlChar     * version,
                  xmlNode     * copyNode ) 
{
   char timeBuf[30];
#if defined (WIN32)
   char tmpTime[9];
#else
   time_t t;
   struct tm * formattedTime;
#endif
   xmlNode * node;
   
   memset( timeBuf, '\0', 30 );

#if defined (WIN32)
   _strdate( timeBuf );
   _strtime( tmpTime );
   strcat( timeBuf, " " );
   strcat( timeBuf, tmpTime );
#else
   t = time(NULL);
   formattedTime = localtime( &t );
   strftime( timeBuf, 30, "%a %b %e %H:%M:%S %Y", formattedTime );
#endif

   // loop on map for addTop()
      
   if ( copyNode != NULL ) {
      /* We extract the copyright element to be able to print them. */
      node = copyNode->children;
      while ( node != NULL ) {
         if ( node->type == XML_ELEMENT_NODE ) {
            // loop on map for addCopyright
         }
         node = node->next;
      }
   }

}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void navigate( errp_common * commonArgs,
               xmlNode     * root )
{
   xmlNode * node = NULL, * group = NULL;
   xmlChar * version;
   
	version = xmlGetProp( root, BAD_CAST "version" );
   
   node = root->children;
   
   /* Go to the first element */
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }

   /* Copyrigth information is optional */
   if ( xmlStrcmp( node->name, BAD_CAST "copyright_group") == 0 ) {
      doTopOfFile( commonArgs, version, node );
      node = node->next;
   }
   else {
      doTopOfFile( commonArgs, version, NULL );
   }

   if ( version != NULL ) xmlFree(version);

   // loop on map for addEndTop
   
   // loop on map for startHeaderGuard
   
   // loop on map for addTopCode
   
   /* 
    * enum information is only present if the target is an enum. If not
    * present, we use "#define" instead.
    */
   if ( commonArgs->writingEnum == 1 ) {
      fprintf( commonArgs->fpHeader, "enum %s\n{\n", commonArgs->enumname );
   }
   
   /*
    * This one is a bit special - we have to know if it's the last group 
    * or not (for enums, the last error of the last group is special - it's
    * the only one not terminated by a comma).
    *
    * So addGroup() is always adding the group of the previous iteration!
    */
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( group != NULL ) addGroup( commonArgs, group, 0 );
         group = node;
      }
      node = node->next; 
   }
   addGroup( commonArgs, group, 1 );

   // loop on map for addBottomCode()
   
   if ( commonArgs->writingEnum == 1 ) {
      fprintf( commonArgs->fpHeader, "};\n\n" );
      fprintf( commonArgs->fpHeader, "typedef enum %s %s;\n\n", 
         commonArgs->enumname, commonArgs->enumname );
   }

   writeErrorMessage( commonArgs );

   // loop on map for stopHeaderGuard
}
#endif

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

int main( int argc, char * argv[] )
{
   xmlParserCtxtPtr context = NULL;  /* The parser context */
   xmlNode * root = NULL;            /* The root node */
   int rc;
   struct errp_common commonArgs;
   char filename[PATH_MAX];
   FILE * fp_dummy;
   vector<AbstractHandler> handlers;

   /*
    * this initialize the library and check potential ABI mismatches
    * between the version it was compiled for and the actual shared
    * library used.
    */
   LIBXML_TEST_VERSION

   memset( &commonArgs, 0, sizeof(struct errp_common) );
   
   rc = handleOptions( handlers, &commonArgs, argc, argv );
   if ( rc != 0 ) {
      if ( rc == 1 ) return 0; /* help */
      return 1;
   }
   
#if 0
   buildPath( filename, commonArgs.headerDir, commonArgs.headerName );
   commonArgs.fpHeader = fopen( filename, "w" );
   if ( commonArgs.fpHeader == NULL ) {
      fprintf( stderr, "Error opening the header file %s.\n", filename );
      return 1;
   }
   buildPath( filename, commonArgs.outputDir, commonArgs.outputNameH );
   commonArgs.fpMsgH = fopen( filename, "w" );
   if ( commonArgs.fpMsgH == NULL ) {
      fprintf( stderr, "Error opening the error message header file %s.\n", 
         filename );
      return 1;
   }
   buildPath( filename, commonArgs.outputDir, commonArgs.outputNameC );
   commonArgs.fpMsgC = fopen( filename, "w" );
   if ( commonArgs.fpMsgC == NULL ) {
      fprintf( stderr, "Error opening the error message C file %s.\n", 
         filename );
      return 1;
   }

   if ( commonArgs.using_cs ) {
      commonArgs.fpCS = fopen( (char *)commonArgs.cs_filename, "w" );
      if ( commonArgs.fpCS == NULL ) {
         fprintf( stderr, "Error opening the C# file %s.\n", 
            commonArgs.cs_filename );
         return 1;
      }
   }

   if ( commonArgs.using_py ) {
      buildPath( filename, commonArgs.py_dirname, commonArgs.py_filename );
      fp_dummy = fopen( filename, "w" );
      if ( fp_dummy == NULL ) {
         fprintf( stderr, "Error opening the Python file %s.\n", 
            filename );
         return 1;
      }
      if ( commonArgs.using_py_extended ) commonArgs.fpPyH = fp_dummy;
      else commonArgs.fpPyPy = fp_dummy;
   }
#endif

   context = xmlNewParserCtxt();
   if ( context == NULL ) {
      fprintf(stderr, "Error allocating the parser context\n");
      return 1;
   }

   /* We create the document and validate in one go */
   commonArgs.document = xmlCtxtReadFile( context, 
                                          (char*)commonArgs.xmlFileName, 
                                          NULL,
                                          XML_PARSE_DTDVALID );
   if ( commonArgs.document == NULL ) {
      cerr << "Error while parsing the input file: " << 
         commonArgs.xmlFileName << endl;
      return 1;
   }
   if ( context->valid == 0 ) {
      cerr << "Error: document validation failed" << endl;
      return 1;
   }
   
   root = xmlDocGetRootElement( commonArgs.document );

//   navigate( &commonArgs, root );

   xmlFreeDoc( commonArgs.document );

   xmlFreeParserCtxt( context );

   xmlCleanupParser();

   fclose( commonArgs.fpHeader );
   fclose( commonArgs.fpMsgC );
   fclose( commonArgs.fpMsgH );
   if (commonArgs.fpCS) fclose( commonArgs.fpCS );
   if (commonArgs.fpPyH) fclose( commonArgs.fpPyH );
   if (commonArgs.fpPyPy) fclose( commonArgs.fpPyPy );

   if ( commonArgs.prefix != NULL ) free(commonArgs.prefix);
      
   return 0;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

