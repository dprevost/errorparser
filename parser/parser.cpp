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

#include <string.h> // memcmp

#include "parser.h"
#include "AbstractHandler.h"

using namespace std;

bool handleOptions( vector<AbstractHandler *> & handlers, 
                    string                    & xmlFileName,
                    string                    & xmlOptionName,
                    string                    & language );

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void usage( char * progName ) 
{
   cerr << "Usage:" << endl << endl;
   cerr << progName << "-o|--options options_xml_file  input_xml_file" << endl;
   cerr << "  or" << endl;
   cerr << progName << " -h|-?|--help" << endl;
   cerr << "  or" << endl;
   cerr << progName << " -v|--version" << endl << endl;
   cerr << "Options:" << endl << endl;
   cerr << "  -o,--options   the name of the xml file defining the options that will be" << endl;
   cerr << "                 used to generate the code." << endl;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void addGroup( string      & language,
               xmlNode     * group,
               bool          lastGroup,
               vector<AbstractHandler *> & handlers )
{
   xmlNode * node = NULL, * errorNode = NULL;
   vector<AbstractHandler *>::iterator it;

   node = group->children;

   // Go to the first element
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }

   /*
    * The error-group identifier is optional. But if present, there might be
    * multiple versions of it (each with a different xml:lang attribute).
    */
   if ( xmlStrcmp( node->name, BAD_CAST "errgroup_ident") == 0 ) {
      
      // node is passed by reference - it will be set correctly (to the 
      // next item to process in the xml tree) when this constructor ends.
      GroupIdent ident( language, node );

      for ( it = handlers.begin(); it < handlers.end(); it++ ) {
         (*it)->addGroupIdent(ident);
      }
   }

   // This one is a bit special - we have to know if it's the last error
   // or not (for enums, the last error of the last group is special - it's
   // the only one not terminated by a comma).
   //
   // So addError() is always adding the error of the previous iteration!
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( errorNode != NULL ) {
            ErrorXML error( language, errorNode );
            for ( it = handlers.begin(); it < handlers.end(); it++ ) {
               (*it)->addError( error, false );
            }
         }
         errorNode = node;
      }
      node = node->next; 
   }
   // The last one of this group. If it is the last group, then it is the
   // the last error, evidently
   {
      ErrorXML error( language, errorNode );
      for ( it = handlers.begin(); it < handlers.end(); it++ ) {
         (*it)->addError( error, lastGroup );
      }
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void doTopOfFile( string                    & xmlFileName,
                  xmlChar                   * version,
                  xmlNode                   * copyNode,
                  vector<AbstractHandler *> & handlers ) 
{
   char timeBuf[30];
   time_t t;
   struct tm * formattedTime;
   xmlNode * node;
   vector<AbstractHandler *>::iterator it;
   
   memset( timeBuf, '\0', 30 );

   t = time(NULL);
   formattedTime = localtime( &t );
   strftime( timeBuf, 30, "%a %b %d %H:%M:%S %Y", formattedTime );

   for ( it = handlers.begin(); it < handlers.end(); it++ ) {
      (*it)->addTop( xmlFileName, timeBuf, (const char *)version );
   }
   
   if ( copyNode != NULL ) {
      // We extract the copyright element to be able to print them.
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
   
   for ( it = handlers.begin(); it < handlers.end(); it++ ) {
      (*it)->addEndTop();
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
   
   // Go to the first element
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }

   // Copyrigth information is optional
   if ( xmlStrcmp( node->name, BAD_CAST "copyright_group") == 0 ) {
      doTopOfFile( xmlFileName, version, node, handlers );
      node = node->next;
   }
   else {
      // Do NOT advance the node (node = node->next). The node was not
      // the copyright group so it is the next element and we must
      // process it later on.
      doTopOfFile( xmlFileName, version, NULL, handlers );
   }

   if ( version != NULL ) xmlFree(version);

   for ( it = handlers.begin(); it < handlers.end(); it++ ) {
      (*it)->startHeaderGuard();
      (*it)->addTopCode();
   }
   
   // This one is a bit special - we have to know if it's the last group 
   // or not (for enums, the last error of the last group is special - it's
   // the only one not terminated by a comma).
   //
   // So addGroup() is always adding the group of the previous iteration!
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
   xmlParserCtxtPtr context = NULL;
   xmlNode * root = NULL;
   bool rc;
   vector<AbstractHandler *> handlers;
   string xmlFileName, xmlOptionName;
   string language;
   xmlDoc  * document = NULL;
   vector<AbstractHandler *>::iterator it;
   
   // This macro initializes the libxml library and check potential ABI 
   // mismatches between the version it was compiled for and the actual 
   // shared library used.
   LIBXML_TEST_VERSION

   if ( argc == 2 ) {
      if ( strcmp("--help",argv[1]) == 0 || strcmp("-h",argv[1]) == 0 ||
           strcmp("-?",argv[1]) == 0 ) {
         usage( argv[0] );
         return 0;
      }
      if ( strcmp("--version",argv[1]) == 0 || strcmp("-v",argv[1]) == 0 ) {
         cerr << VERSION << endl;
         return 0;
      }
   }
   if ( argc != 4 ) {
      usage( argv[0] );
      return 1;
   }
   if ( strcmp("--options",argv[1]) != 0 && strcmp("-o",argv[1]) != 0 ) {
      usage( argv[0] );
      return 1;
   }

   xmlFileName = argv[3];
   xmlOptionName = argv[2];
   try {
      rc = handleOptions( handlers, xmlFileName, xmlOptionName, language );
   }
   catch (...) {
      return 1;
   }
   if ( ! rc )  return 1;

   context = xmlNewParserCtxt();
   if ( context == NULL ) {
      cerr << "Error allocating the parser context" << endl;
      return 1;
   }

   // We create the document and validate in one go
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

   try {
      navigate( xmlFileName, language, root, handlers );
   }
   catch (...) {
      cerr << "Exception caught - most likely an error while writing to one of the output files" << endl;
      return 1;
   }
   
   xmlFreeDoc( document );

   xmlFreeParserCtxt( context );

   xmlCleanupParser();

   for ( it = handlers.begin(); it < handlers.end(); it++ ) {
      delete (*it);
   }

   return 0;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

