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

// Microsoft specific. Must be defined before including system header
// files (this will avoid a warning if we ever use a C library function 
// declared by Microsoft as deprecated.
#define _CRT_SECURE_NO_DEPRECATE

#include <string>
#include <fstream>
#include <iostream>

#include "ErrorXML.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

ErrorXML::ErrorXML( string  & language,
                    xmlNode * errorNode ) throw( MissingException )
   : messageNode( NULL ),
     iterator   ( NULL )
{
   xmlNode * node = NULL;
   
   node = errorNode->children;

   // Go to the first element (error number)
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   stripText( node->children->content, errorNumber );
   if ( errorNumber.empty() ) throw new MissingException( "<errnumber>" );

   // Second element (the error name)
   do { node = node->next; } while ( node->type != XML_ELEMENT_NODE );
   stripText( node->children->content, errorName );
   if ( errorName.empty() ) throw new MissingException( "<errname>" );
   
   do { node = node->next; } while ( node->type != XML_ELEMENT_NODE );

   GetMessageNode( language, node );
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

const char * ErrorXML::GetDocuParagraph()
{
   xmlNode * node;

   if ( iterator == NULL ) {
      node = messageNode->children;
   
      while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   
      // jump over the error message - we only want the docu itself
      node = node->next;
   }
   else {
      node = iterator->next;
   }

   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         iterator = node;
         return (const char *)node->children->content;
      }
      node = node->next;
   }

   // we're done. Reset the iterator.
   iterator = NULL;
   
   return NULL;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

const char * ErrorXML::GetErrMessage()
{
   xmlNode * node;
   
   node = messageNode->children;
   
   // The error message is the first subtag of the messageNode
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }

   return (const char *)node->children->content;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void ErrorXML::GetMessageNode( string  & language,
                               xmlNode * message_group )
{
   xmlNode * node = NULL, * firstNode, * chosenNode = NULL;
   xmlChar * prop;
   
   node = message_group->children;

   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   firstNode = node;

   if ( language.length() != 0 ) {
      // This check on the first element is tedious but is needed in case
      // someone uses the same "xml:lang="XX" for the first <message> and
      // for a subsequent one. In such a case, the first one is the right 
      // one.
      prop = xmlGetProp( node, BAD_CAST "lang" );
      if ( prop != NULL ) {
         if ( xmlStrcmp(prop, BAD_CAST language.c_str()) == 0 ) chosenNode = node;
         xmlFree(prop);
      }
      
      if ( chosenNode == NULL ) {
         while ( node != NULL ) {
            if ( node->type == XML_ELEMENT_NODE ) {
               prop = xmlGetProp( node, BAD_CAST "lang" );
               if ( prop != NULL ) {
                  if ( xmlStrcmp(prop, BAD_CAST language.c_str()) == 0 ) {
                     chosenNode = node;
                     xmlFree(prop);
                     break;
                  }
                  xmlFree(prop);
               }
            }
            node = node->next; 
         }
      }
   }
   
   if ( chosenNode == NULL ) messageNode = firstNode;
   else messageNode = chosenNode;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
