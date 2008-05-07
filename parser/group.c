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

xmlChar * prettify( xmlChar * inStr, char * prefix, int lineLength );
xmlChar * stripText( xmlChar * inStr );

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void addDocumentation( errp_common * commonArgs,
                       xmlNode     * docu )
{
   xmlNode * node = NULL;
   xmlChar * paragraph, * tmp1;
   
   node = docu->children;
   
   do {
      if ( node->type == XML_ELEMENT_NODE ) {
         /* This can only be a paragraph */
         tmp1 = stripText( node->children->content );

         if ( commonArgs->writingEnum ) {
            paragraph = prettify( tmp1, "     * ", ERRP_LINE_LENGTH );
            fprintf( commonArgs->fpHeader, "     *\n%s\n", paragraph );
         }
         else {
            paragraph = prettify( tmp1, " * ", ERRP_LINE_LENGTH );
            fprintf( commonArgs->fpHeader, " *\n%s\n", paragraph );
         }
         free( paragraph );
         free( tmp1 );
      }
      
      node = node->next;

   } while ( node != NULL );
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void addMessages( errp_common * commonArgs,
                  xmlNode     * messages )
{
   xmlNode * node = NULL;
   xmlChar * errMessage, * tmp;
   
   node = messages->children;
   
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   errMessage = stripText( node->children->content );

   if ( commonArgs->writingEnum ) {
      fprintf( commonArgs->fpHeader, "    /**\n     * %s\n", errMessage );
   }
   else {
      fprintf( commonArgs->fpHeader, "/**\n * %s\n", errMessage );
   }
   
   /* Check for escape sequences */
   hasEscapeSequence( commonArgs, errMessage );

   if ( hasUnescapedQuotes(errMessage) ) {
      if ( commonArgs->allowQuotes == 0 ) {
         fprintf( stderr, "Quotes are not allowed, string: %s\n", errMessage );
         exit(1);
      }
      tmp = escapeUnescapedQuotes( errMessage );
      fprintf( commonArgs->fpMsgC, "\"%s\" };\n\n", tmp );
      free( tmp );
   }
   else {
      fprintf( commonArgs->fpMsgC, "\"%s\" };\n\n", errMessage );
   }
   
   node = node->next;
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( xmlStrcmp( node->name, BAD_CAST "documentation") == 0 ) {
            addDocumentation( commonArgs, node );
            break;
         }
      }
      node = node->next;
   }

   if ( commonArgs->writingEnum ) {
      fprintf( commonArgs->fpHeader, "     */\n" );
   }
   else {
      fprintf( commonArgs->fpHeader, " */\n" );
   }
   
   free( errMessage );
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void doMessageGroup( errp_common * commonArgs,
                     xmlNode     * message_group )
{
   xmlNode * node = NULL, * firstNode, * chosenNode = NULL;
   xmlChar * prop;
   
   node = message_group->children;

   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   firstNode = node;

   if ( commonArgs->language != NULL ) {
      /* 
       * This check on the first element is tedious but is needed in case
       * someone uses the same "xml:lang="XX" for the first <message> and
       * for a subsequent one. In such a case, the first one is the right one.
       */
      prop = xmlGetProp( node, BAD_CAST "lang" );
      if ( prop != NULL ) {
         if ( xmlStrcmp(prop, commonArgs->language) == 0 ) chosenNode = node;
         xmlFree(prop);
      }
      
      if ( chosenNode == NULL ) {
         while ( node != NULL ) {
            if ( node->type == XML_ELEMENT_NODE ) {
               prop = xmlGetProp( node, BAD_CAST "lang" );
               if ( prop != NULL ) {
                  if ( xmlStrcmp(prop, commonArgs->language) == 0 ) {
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
   
   if ( chosenNode == NULL ) chosenNode = firstNode;
   addMessages( commonArgs, chosenNode );
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void addError( errp_common * commonArgs,
               xmlNode     * error )
{
   xmlNode * node = NULL;
   xmlChar * errNumber, * errName;
   
   node = error->children;

   /* Go to the first element (error number) */
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   errNumber = stripText( node->children->content );

   /* Second element (the error name) */
   do { node = node->next; } while ( node->type != XML_ELEMENT_NODE );
   errName = stripText( node->children->content );
   
   fprintf( commonArgs->fpMsgC, "/* %s_%s */\n", commonArgs->prefix, errName );
   fprintf( commonArgs->fpMsgC, "%s_MsgStruct %s_Msg%d = {\n", 
      commonArgs->varPrefix, commonArgs->varPrefix, commonArgs->errorCount );
   fprintf( commonArgs->fpMsgC, "    %s, ", errNumber );

   do { node = node->next; } while ( node->type != XML_ELEMENT_NODE );
   doMessageGroup( commonArgs, node );
   
   if ( commonArgs->writingEnum ) {
      fprintf( commonArgs->fpHeader, "    %s_%s = %s",
               commonArgs->prefix, 
               errName,
               errNumber );
   }
   else {
      fprintf( commonArgs->fpHeader, "#define %s_%s %s",
               commonArgs->prefix, 
               errName,
               errNumber );
   }

   free( errNumber );
   free( errName );

   commonArgs->errorCount++;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void addGroup( errp_common * commonArgs,
               xmlNode     * group,
               int           lastGroup )
{
   xmlNode * node = NULL;
   xmlChar * comment, * tmp;
   int firstOfGroup = 1;
   node = group->children;

   /* Go to the first element */
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }

   /* groupident information is optional */
   if ( xmlStrcmp( node->name, BAD_CAST "groupident") == 0 ) {
#if 0
For the moment, we do nothing with groupident 
      tmp = stripText( node->children->content );
      if ( commonArgs->writingEnum ) {
         fprintf( commonArgs->fpHeader, "    /*\n" );
         comment = prettify( tmp, "     * ", ERRP_LINE_LENGTH );
         fprintf( commonArgs->fpHeader, "%s\n", comment );
         fprintf( commonArgs->fpHeader, "     */\n\n" );
      }
      else {
         fprintf( commonArgs->fpHeader, "/*\n" );
         comment = prettify( tmp, " * ", ERRP_LINE_LENGTH );
         fprintf( commonArgs->fpHeader, "%s\n", comment );
         fprintf( commonArgs->fpHeader, " */\n\n" );
      }
#endif
      do { 
         node = node->next;
      } while ( node->type != XML_ELEMENT_NODE );
   }

   /* groupdescript information is optional */
   if ( xmlStrcmp( node->name, BAD_CAST "groupdescript") == 0 ) {
      tmp = stripText( node->children->content );
      if ( commonArgs->writingEnum ) {
         fprintf( commonArgs->fpHeader, "    /*\n" );
         comment = prettify( tmp, "     * ", ERRP_LINE_LENGTH );
         fprintf( commonArgs->fpHeader, "%s\n", comment );
         fprintf( commonArgs->fpHeader, "     */\n\n" );
      }
      else {
         fprintf( commonArgs->fpHeader, "/*\n" );
         comment = prettify( tmp, " * ", ERRP_LINE_LENGTH );
         fprintf( commonArgs->fpHeader, "%s\n", comment );
         fprintf( commonArgs->fpHeader, " */\n\n" );
      }

      do { 
         node = node->next;
      } while ( node->type != XML_ELEMENT_NODE );
   }

   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( ! firstOfGroup ) {
            if ( commonArgs->writingEnum ) {
               fprintf( commonArgs->fpHeader, ",\n\n" );
            }
            else {
               fprintf( commonArgs->fpHeader, "\n\n" );
            }
         }
         addError( commonArgs, node );
         firstOfGroup = 0;
      }
      node = node->next; 
   }
   if ( lastGroup ) {
      if ( commonArgs->writingEnum ) {
         fprintf( commonArgs->fpHeader, "\n" );
      }
      else {
         fprintf( commonArgs->fpHeader, "\n\n" );
      }
   }
   else {
      if ( commonArgs->writingEnum ) {
         fprintf( commonArgs->fpHeader, ",\n\n" );
      }
      else {
         fprintf( commonArgs->fpHeader, "\n\n" );
      }
   }      

   commonArgs->groupCount++;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */
