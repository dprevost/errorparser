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

using namespace std;

void addGroup( errp_common * commonArgs, xmlNode * group, int last );

int handleOptions( vector<AbstractHandler *> & handlers, 
                   errp_common               * commonArgs, 
                   int                         argc, 
                   char                      * argv[] );

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void doTopOfFile( errp_common               * commonArgs,
                  xmlChar                   * version,
                  xmlNode                   * copyNode,
                  vector<AbstractHandler *> & handlers ) 
{
   char timeBuf[30];
#if defined (WIN32)
   char tmpTime[9];
#else
   time_t t;
   struct tm * formattedTime;
#endif
   xmlNode * node;
   vector<AbstractHandler *>::iterator it;
   
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
   for ( it = handlers.begin(); it < handlers.end(); it++ ) {
      (*it)->addTop(commonArgs->xmlFileName, timeBuf, version );
   }
   
   if ( copyNode != NULL ) {
      /* We extract the copyright element to be able to print them. */
      node = copyNode->children;
      while ( node != NULL ) {
         if ( node->type == XML_ELEMENT_NODE ) {
            // loop on map for addCopyright
            for ( it = handlers.begin(); it < handlers.end(); it++ ) {
                (*it)->addCopyright(node);
            }
         }
         node = node->next;
      }
   }

}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void navigate( errp_common               * commonArgs,
               xmlNode                   * root,
               vector<AbstractHandler *> & handlers )
{
   xmlNode * node = NULL, * group = NULL;
   xmlChar * version;
   vector<AbstractHandler *>::iterator it;
   
	version = xmlGetProp( root, BAD_CAST "version" );
   
   node = root->children;
   
   /* Go to the first element */
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }

   /* Copyrigth information is optional */
   if ( xmlStrcmp( node->name, BAD_CAST "copyright_group") == 0 ) {
      doTopOfFile( commonArgs, version, node, handlers );
      node = node->next;
   }
   else {
      doTopOfFile( commonArgs, version, NULL, handlers );
   }

   if ( version != NULL ) xmlFree(version);

   // loop on map for addEndTop
   for ( it = handlers.begin(); it < handlers.end(); it++ ) {
      (*it)->addEndTop();
      (*it)->startHeaderGuard();
      (*it)->addTopCode();
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
//         if ( group != NULL ) addGroup( commonArgs, group, 0 );
         group = node;
      }
      node = node->next; 
   }
//   addGroup( commonArgs, group, 1 );

   // loop on map for addBottomCode()
   
   // loop on map for stopHeaderGuard
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

int main( int argc, char * argv[] )
{
   xmlParserCtxtPtr context = NULL;  /* The parser context */
   xmlNode * root = NULL;            /* The root node */
   int rc;
   struct errp_common commonArgs;
   vector<AbstractHandler *> handlers;

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

   context = xmlNewParserCtxt();
   if ( context == NULL ) {
      fprintf(stderr, "Error allocating the parser context\n");
      return 1;
   }

   /* We create the document and validate in one go */
   commonArgs.document = xmlCtxtReadFile( context, 
                                          commonArgs.xmlFileName.c_str(), 
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

   return 0;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

