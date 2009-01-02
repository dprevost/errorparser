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

#include <vector>

#include "parser.h"
#include "AbstractHandler.h"

using namespace std;

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void addGroupIdentifier( xmlNode                   * ident,
                         vector<AbstractHandler *> & handlers )
{
   xmlNode * node;
   vector<AbstractHandler *>::iterator it;
   string tmp;
   
   node = ident->children;

   /* Go to the first element, errgroup_name */
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }

   stripText( node->children->content, tmp );
   for ( it = handlers.begin(); it < handlers.end(); it++ ) {
      (*it)->addGroupName( tmp );
   }
   
   do { 
      node = node->next;
   } while ( node->type != XML_ELEMENT_NODE );

   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         stripText( node->children->content, tmp );
         for ( it = handlers.begin(); it < handlers.end(); it++ ) {
            (*it)->addGroupDesc( tmp );
         }
      }
      node = node->next;
   }

   for ( it = handlers.begin(); it < handlers.end(); it++ ) {
      (*it)->endGroupDesc();
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

void addGroup( string      & language,
               xmlNode     * group,
               bool          lastGroup,
               vector<AbstractHandler *> & handlers )
{
   xmlNode * node = NULL, * firstNode, * chosenNode = NULL;
   xmlChar * prop;
   int firstOfGroup = 1;
   node = group->children;
   vector<AbstractHandler *>::iterator it;

   /* Go to the first element */
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }

   /*
    * The error-group identifier is optional. But if there, there could be
    * multiple versions of it (each with a different xml:lang attribute).
    */
   if ( xmlStrcmp( node->name, BAD_CAST "errgroup_ident") == 0 ) {
      firstNode = node;
      node = node->next;

      if ( language.length() != 0 ) {
         prop = xmlGetProp( node, BAD_CAST "lang" );
         if ( prop != NULL ) {
            if ( xmlStrcmp(prop, BAD_CAST language.c_str()) == 0 ) {
               chosenNode = node;
            }
            xmlFree(prop);
         }
      
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
   
      if ( chosenNode == NULL ) chosenNode = firstNode;
      addGroupIdentifier( chosenNode, handlers );    
   }

   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( ! firstOfGroup ) {
            for ( it = handlers.begin(); it < handlers.end(); it++ ) {
               (*it)->addErrorTrailer();
            }
         }
         ErrorXML error( language, node );
         for ( it = handlers.begin(); it < handlers.end(); it++ ) {
             (*it)->addError( error );
         }
         firstOfGroup = 0;
      }
      node = node->next; 
   }
   if ( ! lastGroup ) {
      for ( it = handlers.begin(); it < handlers.end(); it++ ) {
         (*it)->addErrorTrailer();
      }
   }
}

// --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

