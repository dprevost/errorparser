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

#include "GroupIdent.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

GroupIdent::GroupIdent( string    & language,
                        xmlNode * & identNode )
   : textNode( NULL ),
     iterator( NULL )
{
   xmlNode * node = identNode;
   xmlNode * firstNode, * chosenNode = NULL;
   xmlChar * prop;

   firstNode = node;

   // First task: identify which "errgroup_ident" matches the requested
   // language (if any). The first "errgroup_ident" (firstNode) will be 
   // chosen if no language matches or if "language" is not used.
   if ( language.length() != 0 ) {
      prop = xmlGetProp( node, BAD_CAST "lang" );
      if ( prop != NULL ) {
         if ( xmlStrcmp(prop, BAD_CAST language.c_str()) == 0 ) {
            chosenNode = node;
         }
         xmlFree(prop);
      }
      node = node->next;
      
      if ( chosenNode == NULL ) {
         while ( node != NULL ) {
            if ( node->type == XML_ELEMENT_NODE ) {
               if ( xmlStrcmp( node->name, BAD_CAST "errgroup_ident") != 0 ) break;

               prop = xmlGetProp( node, BAD_CAST "lang" );
               if ( prop != NULL ) {
                  if ( xmlStrcmp(prop, BAD_CAST language.c_str()) == 0 ) {
                     chosenNode = node;
                     xmlFree(prop);
                     node = node->next;
                     break;
                  }
                  xmlFree(prop);
               }
            }
            node = node->next; 
         }
      }
   }
   else {
      node = node->next;
   }

   // This value is "returned" to the caller so that it can go on to 
   // scan the xml tree from this node.
   identNode = node;

   if ( chosenNode == NULL ) chosenNode = firstNode;

   node = chosenNode->children;

   // Go to the first element, the name
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }

   stripText( node->children->content, name );
   
   do { 
      node = node->next;
   } while ( node->type != XML_ELEMENT_NODE );

   // The remaining nodes are the paragraphs of the description
   textNode = node;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

const char * GroupIdent::GetDescParagraph()
{
   xmlNode * node;

   if ( iterator == NULL ) {
      node = textNode;
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

   // we already processed the last paragraph.
   iterator = NULL;
   
   return NULL;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
