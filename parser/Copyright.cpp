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

#include <iostream>
#include "Copyright.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

Copyright::Copyright( xmlNode * errorNode )
   : textNode( NULL ),
     iterator( NULL )
{
   xmlNode * node = NULL;
   
   node = errorNode->children;

   /* Go to the first element (years) */
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   stripText( node->children->content, years );

   /* Second element (authors) */
   do { node = node->next; } while ( node->type != XML_ELEMENT_NODE );
   stripText( node->children->content, authors );
   
   do { node = node->next; } while ( node->type != XML_ELEMENT_NODE );

   textNode = node;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

xmlChar * Copyright::GetCopyParagraph()
{
   xmlNode * node;

   if ( iterator == NULL ) {
      iterator = textNode;
      return iterator->children->content;
   }
   
   node = iterator->next;

   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         iterator = node;
         return node->children->content;
      }
      node = node->next;
   }

   // we processed the last paragraph
   iterator = NULL;
   
   return NULL;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
