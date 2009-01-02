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

#include "parser.h"

#include <vector>
#include <iostream>
#include "AbstractHandler.h"

using namespace std;

void addGroup( string & language, xmlNode * group, bool last, 
               vector<AbstractHandler *> & handlers);

int handleOptions( vector<AbstractHandler *> & handlers, 
                   string                    & xmlFileName,
                   string                    & language,
                   int                         argc,
                   char                      * argv[] );

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void doTopOfFile( string                    & xmlFileName,
                  xmlChar                   * version,
                  xmlNode                   * copyNode,
                  vector<AbstractHandler *> & handlers ) 
{
   char timeBuf[30];
#if defined (WIN32qqq)
   char tmpTime[9];
#else
   time_t t;
   struct tm * formattedTime;
#endif
   xmlNode * node;
   vector<AbstractHandler *>::iterator it;
   
   memset( timeBuf, '\0', 30 );

#if defined (WIN32qqq)
   _strdate( timeBuf );
   _strtime( tmpTime );
   strcat( timeBuf, " " );
   strcat( timeBuf, tmpTime );
#else
   t = time(NULL);
   formattedTime = localtime( &t );
   strftime( timeBuf, 30, "%a %b %e %H:%M:%S %Y", formattedTime );
#endif
   for ( it = handlers.begin(); it < handlers.end(); it++ ) {
      (*it)->addTop( xmlFileName, timeBuf, version );
   }
   
   if ( copyNode != NULL ) {
      /* We extract the copyright element to be able to print them. */
      node = copyNode->children;
      while ( node != NULL ) {
         if ( node->type == XML_ELEMENT_NODE ) {
            Copyright copy(node);
            for ( it = handlers.begin(); it < handlers.end(); it++ ) {
                (*it)->addCopyright(copy);
            }
         }
         node = node->next;
      }
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void navigate( string                    & xmlFileName,
               string                    & language,
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
      doTopOfFile( xmlFileName, version, node, handlers );
      node = node->next;
   }
   else {
      doTopOfFile( xmlFileName, version, NULL, handlers );
   }

   if ( version != NULL ) xmlFree(version);

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
         if ( group != NULL ) addGroup( language, group, false, handlers );
         group = node;
      }
      node = node->next; 
   }
   addGroup( language, group, true, handlers );

   for ( it = handlers.begin(); it < handlers.end(); it++ ) {
      (*it)->addBottomCode();
      (*it)->stopHeaderGuard();
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

int main( int argc, char * argv[] )
{
   xmlParserCtxtPtr context = NULL;  /* The parser context */
   xmlNode * root = NULL;            /* The root node */
   int rc;
   vector<AbstractHandler *> handlers;
   string xmlFileName;
   string language;
   xmlDoc  * document = NULL;
   
   /*
    * this initialize the library and check potential ABI mismatches
    * between the version it was compiled for and the actual shared
    * library used.
    */
   LIBXML_TEST_VERSION

   rc = handleOptions( handlers, xmlFileName, language, argc, argv );
   if ( rc != 0 ) {
      if ( rc == 1 ) return 0; /* help */
      return 1;
   }

   context = xmlNewParserCtxt();
   if ( context == NULL ) {
      cerr << "Error allocating the parser context" << endl;
      return 1;
   }

   /* We create the document and validate in one go */
   document = xmlCtxtReadFile( context, 
                               xmlFileName.c_str(), 
                               NULL,
                               XML_PARSE_DTDVALID );
   if ( document == NULL ) {
      cerr << "Error while parsing the input file: " << 
         xmlFileName << endl;
      return 1;
   }
   if ( context->valid == 0 ) {
      cerr << "Error: document validation failed" << endl;
      return 1;
   }
   
   root = xmlDocGetRootElement( document );

   navigate( xmlFileName, language, root, handlers );

   xmlFreeDoc( document );

   xmlFreeParserCtxt( context );

   xmlCleanupParser();

   return 0;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

