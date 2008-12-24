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

#include "parser1.h"
#include "AbstractHandler.h"

using namespace std;

//xmlChar * prettify( xmlChar * inStr, char * prefix, int lineLength );
//xmlChar * stripText( xmlChar * inStr,  );

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */
#if 0

void addDocumentation( errp_common * commonArgs,
                       xmlNode     * node )
{
   xmlChar * paragraph, * tmp1;
   int firstpara = 1;
   
   if ( commonArgs->writingEnum ) {
      fprintf( commonArgs->fpHeader, "    /**\n" );
   }
   else {
      fprintf( commonArgs->fpHeader, "/**\n" );
   }

   do {
      if ( node->type == XML_ELEMENT_NODE ) {
         /* This can only be a paragraph of the documentation */
         tmp1 = stripText( node->children->content );

         if ( firstpara ) firstpara = 0;
         else {
            if ( commonArgs->writingEnum ) {
               fprintf( commonArgs->fpHeader, "     *\n" );
            }
            else {
               fprintf( commonArgs->fpHeader, " *\n" );
            }
         }
         
         if ( commonArgs->writingEnum ) {
            paragraph = prettify( tmp1, "     * ", ERRP_LINE_LENGTH );
         }
         else {
            paragraph = prettify( tmp1, " * ", ERRP_LINE_LENGTH );
         }
         fprintf( commonArgs->fpHeader, "%s\n", paragraph );

         free( paragraph );
         free( tmp1 );
      }
      
      node = node->next;

   } while ( node != NULL );
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void addDocumentationCS( errp_common * commonArgs,
                         xmlNode     * node )
{
   xmlChar * paragraph, * tmp1;
   int firstpara = 1;
   
   do {
      if ( node->type == XML_ELEMENT_NODE ) {
         /* This can only be a paragraph of the documentation */
         tmp1 = stripText( node->children->content );
         
         if ( firstpara ) firstpara = 0;
         else {
            if ( commonArgs->cs_namespace == NULL ) {
               fprintf( commonArgs->fpCS, "    //\n" );
            }
            else {
               fprintf( commonArgs->fpCS, "        //\n" );
            }
         }
         if ( commonArgs->cs_namespace == NULL ) {
            paragraph = prettify( tmp1, "    // ", ERRP_LINE_LENGTH );
         }
         else {
            paragraph = prettify( tmp1, "        // ", ERRP_LINE_LENGTH );
         }
         fprintf( commonArgs->fpCS, "%s\n", paragraph );

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
   
   node = node->next;
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         addDocumentation( commonArgs, node );
         if ( commonArgs->using_cs ) addDocumentationCS( commonArgs, node );
         break;
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

#endif

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

xmlNode * getMessageNode( errp_common * commonArgs,
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
   
   return chosenNode;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

#if 0

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

#endif

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void addError( errp_common               * commonArgs,
               xmlNode                   * error,
               vector<AbstractHandler *> & handlers )
{
   xmlNode * node = NULL, * messageNode = NULL;
   string errNumber, errName;
   vector<AbstractHandler *>::iterator it;
   
   node = error->children;

   /* Go to the first element (error number) */
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   stripText( node->children->content, errNumber );

   /* Second element (the error name) */
   do { node = node->next; } while ( node->type != XML_ELEMENT_NODE );
   stripText( node->children->content, errName );
   
   do { node = node->next; } while ( node->type != XML_ELEMENT_NODE );
//   doMessageGroup( commonArgs, node );

   messageNode = getMessageNode( commonArgs, node );
//   addMessages( commonArgs, chosenNode );
   
   for ( it = handlers.begin(); it < handlers.end(); it++ ) {
      (*it)->addError( errNumber, errName, messageNode );
   }

#if 0
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

   if ( commonArgs->using_cs ) {
      if ( commonArgs->cs_namespace == NULL ) {
         fprintf( commonArgs->fpCS, "    %s = %s",
               errName,
               errNumber );
      }
      else
      {
         fprintf( commonArgs->fpCS, "        %s = %s",
                  errName,
                  errNumber );
      }
   }

   if ( commonArgs->fpPyH ) {
      fprintf( commonArgs->fpPyH, "    value = PyInt_FromLong(%s);\n", errNumber );
      fprintf( commonArgs->fpPyH, "    if ( value == NULL ) {\n" );
      fprintf( commonArgs->fpPyH, "        PyDict_Clear(errors);\n" );
      fprintf( commonArgs->fpPyH, "        Py_DECREF(errors);\n" );
      fprintf( commonArgs->fpPyH, "        PyDict_Clear(errorNames);\n" );
      fprintf( commonArgs->fpPyH, "        Py_DECREF(errorNames);\n" );
      fprintf( commonArgs->fpPyH, "        return NULL;\n" );
      fprintf( commonArgs->fpPyH, "    }\n" );
      fprintf( commonArgs->fpPyH, "    key = PyString_FromString(\"%s\");\n", errName );
      fprintf( commonArgs->fpPyH, "    if ( key == NULL ) {\n" );
      fprintf( commonArgs->fpPyH, "        Py_DECREF(value);\n" );
      fprintf( commonArgs->fpPyH, "        PyDict_Clear(errors);\n" );
      fprintf( commonArgs->fpPyH, "        Py_DECREF(errors);\n" );
      fprintf( commonArgs->fpPyH, "        PyDict_Clear(errorNames);\n" );
      fprintf( commonArgs->fpPyH, "        Py_DECREF(errorNames);\n" );
      fprintf( commonArgs->fpPyH, "        return NULL;\n" );
      fprintf( commonArgs->fpPyH, "    }\n" );
      fprintf( commonArgs->fpPyH, "    errcode = PyDict_SetItem(errors, key, value);\n" );
      fprintf( commonArgs->fpPyH, "    if (errcode != 0) {\n" );
      fprintf( commonArgs->fpPyH, "        Py_DECREF(value);\n" );
      fprintf( commonArgs->fpPyH, "        Py_DECREF(key);\n" );
      fprintf( commonArgs->fpPyH, "        PyDict_Clear(errors);\n" );
      fprintf( commonArgs->fpPyH, "        Py_DECREF(errors);\n" );
      fprintf( commonArgs->fpPyH, "        PyDict_Clear(errorNames);\n" );
      fprintf( commonArgs->fpPyH, "        Py_DECREF(errorNames);\n" );
      fprintf( commonArgs->fpPyH, "        return NULL;\n" );
      fprintf( commonArgs->fpPyH, "    }\n" );
      fprintf( commonArgs->fpPyH, "    errcode = PyDict_SetItem(errorNames, value, key);\n" );
      fprintf( commonArgs->fpPyH, "    Py_DECREF(value);\n" );
      fprintf( commonArgs->fpPyH, "    Py_DECREF(key);\n" );
      fprintf( commonArgs->fpPyH, "    if (errcode != 0) {\n" );
      fprintf( commonArgs->fpPyH, "        PyDict_Clear(errors);\n" );
      fprintf( commonArgs->fpPyH, "        Py_DECREF(errors);\n" );
      fprintf( commonArgs->fpPyH, "        PyDict_Clear(errorNames);\n" );
      fprintf( commonArgs->fpPyH, "        Py_DECREF(errorNames);\n" );
      fprintf( commonArgs->fpPyH, "        return NULL;\n" );
      fprintf( commonArgs->fpPyH, "    }\n\n" );
   }

   if ( commonArgs->fpPyPy ) {
      fprintf( commonArgs->fpPyPy, "    errors['%s'] = %s\n", errName, errNumber );
      fprintf( commonArgs->fpPyPy, "    error_names[%s] = '%s'\n", errNumber, errName );
   }
   
   free( errNumber );
   free( errName );

   commonArgs->errorCount++;
#endif
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void addGroupIdentifier( errp_common               * commonArgs,
                         xmlNode                   * ident,
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

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */
#if 0

void addGroup( errp_common * commonArgs,
               xmlNode     * group,
               int           lastGroup )
{
   xmlNode * node = NULL, * firstNode, * chosenNode = NULL;
   xmlChar * prop;
   int firstOfGroup = 1;
   node = group->children;

   /* Go to the first element */
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }

   /*
    * The error-group identifier is optional. But if there, there could be
    * multiple versions of it (each with a different xml:lang attribute).
    */
   if ( xmlStrcmp( node->name, BAD_CAST "errgroup_ident") == 0 ) {
      firstNode = node;
      node = node->next;

      if ( commonArgs->language != NULL ) {
         prop = xmlGetProp( node, BAD_CAST "lang" );
         if ( prop != NULL ) {
            if ( xmlStrcmp(prop, commonArgs->language) == 0 ) chosenNode = node;
            xmlFree(prop);
         }
      
         if ( chosenNode == NULL ) {
            while ( node != NULL ) {
               if ( node->type == XML_ELEMENT_NODE ) {
                  if ( xmlStrcmp( node->name, BAD_CAST "errgroup_ident") != 0 ) break;

                  prop = xmlGetProp( node, BAD_CAST "lang" );
                  if ( prop != NULL ) {
                     if ( xmlStrcmp(prop, commonArgs->language) == 0 ) {
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
      addGroupIdentifier( commonArgs, chosenNode );    
   }

   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         if ( ! firstOfGroup ) {
            if ( commonArgs->writingEnum ) {
               fprintf( commonArgs->fpHeader, ",\n\n" );
               if (commonArgs->using_cs) fprintf( commonArgs->fpCS, ",\n\n" );
            }
            else {
               fprintf( commonArgs->fpHeader, "\n\n" );
               if (commonArgs->using_cs) fprintf( commonArgs->fpCS, "\n\n" );
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
         if (commonArgs->using_cs) fprintf( commonArgs->fpCS, "\n" );
      }
      else {
         fprintf( commonArgs->fpHeader, "\n\n" );
         if (commonArgs->using_cs) fprintf( commonArgs->fpCS, "\n\n" );
      }
   }
   else {
      if ( commonArgs->writingEnum ) {
         fprintf( commonArgs->fpHeader, ",\n\n" );
         if (commonArgs->using_cs) fprintf( commonArgs->fpCS, ",\n\n" );
      }
      else {
         fprintf( commonArgs->fpHeader, "\n\n" );
         if (commonArgs->using_cs) fprintf( commonArgs->fpCS, "\n\n" );
      }
   }      

   commonArgs->groupCount++;
}
#endif

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */
