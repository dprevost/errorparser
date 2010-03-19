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

#include "Copyright.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

Copyright::Copyright( xmlNode * errorNode ) throw( MissingException )
   : textNode( NULL ),
     iterator( NULL )
{
   xmlNode * node = NULL;
   
   node = errorNode->children;

   // Go to the first element (years)
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   stripText( node->children->content, years );
   if ( years.empty() ) throw new MissingException( "<years>" );

   // Second element (authors)
   do { node = node->next; } while ( node->type != XML_ELEMENT_NODE );
   stripText( node->children->content, authors );
   if ( authors.empty() ) throw new MissingException( "<authors>" );
   
   do { node = node->next; } while ( node->type != XML_ELEMENT_NODE );

   textNode = node;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

const char * Copyright::GetCopyParagraph()
{
   xmlNode * node;

   if ( iterator == NULL ) {
      iterator = textNode;
      return (const char *)iterator->children->content;
   }
   
   node = iterator->next;

   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         iterator = node;
         return (const char *)node->children->content;
      }
      node = node->next;
   }

   // we reset the iterator after the last paragraph (node == NULL)
   iterator = NULL;
   
   return NULL;
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
