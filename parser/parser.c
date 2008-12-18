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

void addGroup( errp_common * commonArgs, xmlNode * group, int last );
int handleOptions( errp_common * commonArgs, int argc, char * argv[] );

const char * g_barrier = "/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */";
const char * g_barrierPy = "# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--";
const char * g_functionName = "_ErrorMessage";

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void addCopyright( errp_common * commonArgs, xmlNode * node )
{
   xmlChar * years, * authors, * notice, * tmp, * noticePy;

   node = node->children;
   
   /* Go to the first element */
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }
   years = stripText( node->children->content );
   
   /* Go to the next element */
   do { node = node->next; } while ( node->type != XML_ELEMENT_NODE );
   authors = stripText( node->children->content );
   
   fprintf( commonArgs->fpHeader, " * Copyright (C) %s %s\n", years, authors );
   fprintf( commonArgs->fpMsgC, " * Copyright (C) %s %s\n", years, authors );
   fprintf( commonArgs->fpMsgH, " * Copyright (C) %s %s\n", years, authors );
   if ( commonArgs->using_cs ) {
      fprintf( commonArgs->fpCS, " * Copyright (C) %s %s\n", years, authors );
   }
   if ( commonArgs->fpPyH ) {
      fprintf( commonArgs->fpPyH, " * Copyright (C) %s %s\n", years, authors );
   }
   if ( commonArgs->fpPyPy ) {
      fprintf( commonArgs->fpPyPy, "# Copyright (C) %s %s\n", years, authors );
   }
   
   free( years );
   free( authors );
   
   node = node->next;
   while ( node != NULL ) {
      if ( node->type == XML_ELEMENT_NODE ) {
         fprintf( commonArgs->fpHeader, " *\n" );
         fprintf( commonArgs->fpMsgC, " *\n" );
         fprintf( commonArgs->fpMsgH, " *\n" );
         if ( commonArgs->using_cs ) fprintf( commonArgs->fpCS, " *\n" );
         if ( commonArgs->fpPyH ) fprintf( commonArgs->fpPyH, " *\n" );
         if ( commonArgs->fpPyPy ) fprintf( commonArgs->fpPyPy, "#\n" );

         tmp = stripText( node->children->content );
         notice = prettify( tmp, " * ", ERRP_LINE_LENGTH );
         noticePy = prettify( tmp, "# ", ERRP_LINE_LENGTH );
         
         fprintf( commonArgs->fpHeader, "%s\n", notice );
         fprintf( commonArgs->fpMsgC, "%s\n", notice );
         fprintf( commonArgs->fpMsgH, "%s\n", notice );
         if ( commonArgs->using_cs ) fprintf( commonArgs->fpCS, "%s\n", notice );
         if ( commonArgs->fpPyH ) fprintf( commonArgs->fpPyH, "%s\n", notice );
         if ( commonArgs->fpPyPy ) fprintf( commonArgs->fpPyPy, "%s\n", noticePy );

         free( tmp );
         free( notice );
         free( noticePy );
      }
      node = node->next;
   }
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void startHeaderGuard( const char * name, FILE * fp )
{
   unsigned int i;
   char * tmp = NULL;
   
   tmp = (char *)calloc( strlen(name)+1, sizeof(char) );
   if ( tmp == NULL ) {
      fprintf( stderr, "Malloc error\n" );
      exit(1);
   }
   
   for ( i = 0; i < strlen(name); ++i ) {
      if ( isalnum(name[i]) ) {
         if ( isalpha(name[i]) ) {
            tmp[i] = toupper(name[i]);
         }
         else tmp[i] = name[i];
      }
      else {
         tmp[i] = '_';
      }
   }
   fprintf( fp, "#ifndef %s\n", tmp );
   fprintf( fp, "#define %s\n\n", tmp );
   fprintf( fp, "#ifdef __cplusplus\n" );
   fprintf( fp, "extern \"C\" {\n" );
   fprintf( fp, "#endif\n\n" );
   fprintf( fp, "%s\n\n", g_barrier );
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void stopHeaderGuard( const char * name, FILE * fp )
{
   unsigned int i;
   char * tmp = NULL;
   
   tmp = (char *)calloc( strlen(name)+1, sizeof(char) );
   if ( tmp == NULL ) {
      fprintf( stderr, "Malloc error\n" );
      exit(1);
   }
   
   for ( i = 0; i < strlen(name); ++i ) {
      if ( isalnum(name[i]) ) {
         if ( isalpha(name[i]) ) {
            tmp[i] = toupper(name[i]);
         }
         else tmp[i] = name[i];
      }
      else {
         tmp[i] = '_';
      }
   }
   fprintf( fp, "%s\n\n", g_barrier );
   fprintf( fp, "#ifdef __cplusplus\n" );
   fprintf( fp, "}\n#endif\n\n" );
   fprintf( fp, "#endif /* %s */\n\n", tmp );
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void writeMsgHeader( errp_common * commonArgs )
{
   FILE * fp = commonArgs->fpMsgH; /* To simplify the code */

   if ( commonArgs->build_dll ) {
      /* Windows export/import decoration. */
      fprintf( fp, "#if defined(WIN32)\n" );
      fprintf( fp, "#  if defined(BULDING_ERROR_MESSAGE)\n" );
      fprintf( fp, "#    define ERROR_MESSAGE_EXPORT __declspec ( dllexport )\n" );
      fprintf( fp, "#  else\n" );
      fprintf( fp, "#    define ERROR_MESSAGE_EXPORT __declspec ( dllimport )\n" );
      fprintf( fp, "#  endif\n" );
      fprintf( fp, "#else\n" );
      fprintf( fp, "#  define ERROR_MESSAGE_EXPORT\n" );
      fprintf( fp, "#endif\n\n" );
      fprintf( fp, "%s\n\n", g_barrier );
   }
   
   fprintf( fp, "/*\n" );
   fprintf( fp, "%s\n", 
      " * Use this function to access the error messages (defined in the xml" );
   fprintf( fp, " * input file).\n" );
   fprintf( fp, " * \n" );
   fprintf( fp, " * Parameters:\n" );
   fprintf( fp, " *   - errnum    The error number\n" );
   fprintf( fp, " *\n" );
   fprintf( fp, " * Return values:\n" );
   fprintf( fp, " *   - the error message if errnum is valid (exists)\n" );
   fprintf( fp, " *   - NULL otherwise\n" );
   fprintf( fp, " */\n" );

   if ( commonArgs->build_dll ) fprintf( fp, "ERROR_MESSAGE_EXPORT\n" );
   fprintf( fp, "const char * %s%s( int errnum );\n\n",
      commonArgs->varPrefix, g_functionName );
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void writeTopC(  errp_common * commonArgs )
{
   fprintf( commonArgs->fpMsgC, "#include <stdlib.h> %s\n",
      "/* Any system file will do. Needed for NULL. */" );
   fprintf( commonArgs->fpMsgC, "#define BULDING_ERROR_MESSAGE\n" );

   fprintf( commonArgs->fpMsgC, "#include \"%s\"\n\n", commonArgs->outputNameH );
   fprintf( commonArgs->fpMsgC, "struct %s_MsgStruct\n", commonArgs->varPrefix );
   fprintf( commonArgs->fpMsgC, "{\n" );
   fprintf( commonArgs->fpMsgC, "    int  errorNumber;\n" );
   fprintf( commonArgs->fpMsgC, "    const char * message;\n" );
   fprintf( commonArgs->fpMsgC, "};\n\n" );
   fprintf( commonArgs->fpMsgC, "typedef struct %s_MsgStruct %s_MsgStruct;\n\n",
      commonArgs->varPrefix, commonArgs->varPrefix );
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void writeTopCS( errp_common * commonArgs )
{
   fprintf( commonArgs->fpCS, "using System;\n\n" );
//   fprintf( commonArgs->fpCS, "using System.Collections.Generic;;\n" );
//   fprintf( commonArgs->fpCS, "using System.Text;\n" );
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void writeTopPyH( errp_common * commonArgs )
{
   fprintf( commonArgs->fpPyH, "#include \"Python.h\"\n\n" );
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */
   
void doTopOfFile( errp_common * commonArgs,
                  xmlChar     * version,
                  xmlNode     * copyNode ) 
{
   char timeBuf[30];
#if defined (WIN32)
   char tmpTime[9];
#else
   time_t t;
   struct tm * formattedTime;
#endif
   xmlNode * node;
   
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

   fprintf( commonArgs->fpHeader, "/*\n" );
   fprintf( commonArgs->fpHeader, " * This file was generated by the program errorParser\n" );
   fprintf( commonArgs->fpHeader, " * using the input file %s.\n", commonArgs->xmlFileName );
   fprintf( commonArgs->fpHeader, " * Date: %s.\n *\n", timeBuf );
   if ( version != NULL ) {
      fprintf( commonArgs->fpHeader, " * The version of this interface is %s.\n *\n", version );
   }
   
   fprintf( commonArgs->fpMsgC, "/*\n" );
   fprintf( commonArgs->fpMsgC, " * This file was generated by the program errorParser\n" );
   fprintf( commonArgs->fpMsgC, " * using the input file %s.\n", commonArgs->xmlFileName );
   fprintf( commonArgs->fpMsgC, " * Date: %s.\n *\n", timeBuf );
   if ( version != NULL ) {
      fprintf( commonArgs->fpMsgC, " * The version of this interface is %s.\n *\n", version );
   }
   
   fprintf( commonArgs->fpMsgH, "/*\n" );
   fprintf( commonArgs->fpMsgH, " * This file was generated by the program errorParser\n" );
   fprintf( commonArgs->fpMsgH, " * using the input file %s.\n", commonArgs->xmlFileName );
   fprintf( commonArgs->fpMsgH, " * Date: %s.\n *\n", timeBuf );
   if ( version != NULL ) {
      fprintf( commonArgs->fpMsgH, " * The version of this interface is %s.\n *\n", version );
   }
   
   if ( commonArgs->using_cs ) {
      fprintf( commonArgs->fpCS, "/*\n" );
      fprintf( commonArgs->fpCS, " * This file was generated by the program errorParser\n" );
      fprintf( commonArgs->fpCS, " * using the input file %s.\n", commonArgs->xmlFileName );
      fprintf( commonArgs->fpCS, " * Date: %s.\n *\n", timeBuf );
      if ( version != NULL ) {
         fprintf( commonArgs->fpCS, " * The version of this interface is %s.\n *\n", version );
      }
   }
   if ( commonArgs->fpPyH ) {
      fprintf( commonArgs->fpPyH, "/*\n" );
      fprintf( commonArgs->fpPyH, " * This file was generated by the program errorParser\n" );
      fprintf( commonArgs->fpPyH, " * using the input file %s.\n", commonArgs->xmlFileName );
      fprintf( commonArgs->fpPyH, " * Date: %s.\n *\n", timeBuf );
      if ( version != NULL ) {
         fprintf( commonArgs->fpPyH, " * The version of this interface is %s.\n *\n", version );
      }
   }
   if ( commonArgs->fpPyPy ) {
      fprintf( commonArgs->fpPyPy, "\n" );
      fprintf( commonArgs->fpPyPy, "# This file was generated by the program errorParser\n" );
      fprintf( commonArgs->fpPyPy, "# using the input file %s.\n", commonArgs->xmlFileName );
      fprintf( commonArgs->fpPyPy, "# Date: %s.\n\n", timeBuf );
      if ( version != NULL ) {
         fprintf( commonArgs->fpPyPy, "# The version of this interface is %s.\n\n", version );
      }
   }
   
   if ( copyNode != NULL ) {
      /* We extract the copyright element to be able to print them. */
      node = copyNode->children;
      while ( node != NULL ) {
         if ( node->type == XML_ELEMENT_NODE ) {
            addCopyright( commonArgs, node );
            fprintf( commonArgs->fpHeader, " *\n" );
            fprintf( commonArgs->fpMsgC,   " *\n" );
            fprintf( commonArgs->fpMsgH,   " *\n" );
            if ( commonArgs->using_cs ) fprintf( commonArgs->fpCS, " *\n" );
            if ( commonArgs->fpPyH ) fprintf( commonArgs->fpPyH,   " *\n" );
            if ( commonArgs->fpPyPy ) fprintf( commonArgs->fpPyPy, "\n" );
         }
         node = node->next;
      }
   }
   fprintf( commonArgs->fpHeader, " */\n\n%s\n\n", g_barrier );
   fprintf( commonArgs->fpMsgC, " */\n\n%s\n\n", g_barrier );
   fprintf( commonArgs->fpMsgH, " */\n\n%s\n\n", g_barrier );
   if ( commonArgs->using_cs ) {
      fprintf( commonArgs->fpCS, " */\n\n%s\n\n", g_barrier );
   }
   if ( commonArgs->fpPyH ) {
      fprintf( commonArgs->fpPyH, " */\n\n%s\n\n", g_barrier );
   }
   if ( commonArgs->fpPyPy ) {
      fprintf( commonArgs->fpPyPy, "%s\n\n", g_barrierPy );
   }
   
   startHeaderGuard( (char*)commonArgs->headerName, commonArgs->fpHeader );
   startHeaderGuard( (char*)commonArgs->outputNameH, commonArgs->fpMsgH );
   if ( commonArgs->fpPyH ) {
      startHeaderGuard( (char*)commonArgs->py_filename, commonArgs->fpPyH );
   }
   writeTopC( commonArgs );
   if ( commonArgs->using_cs ) writeTopCS( commonArgs );
   if ( commonArgs->fpPyH ) writeTopPyH( commonArgs );
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void navigate( errp_common * commonArgs,
               xmlNode     * root )
{
   xmlNode * node = NULL, * group = NULL;
   xmlChar * version;
   
	version = xmlGetProp( root, BAD_CAST "version" );
   
   node = root->children;
   
   /* Go to the first element */
   while ( node->type != XML_ELEMENT_NODE ) { node = node->next; }

   /* Copyrigth information is optional */
   if ( xmlStrcmp( node->name, BAD_CAST "copyright_group") == 0 ) {
      doTopOfFile( commonArgs, version, node );
      node = node->next;
   }
   else {
      doTopOfFile( commonArgs, version, NULL );
   }

   if ( version != NULL ) xmlFree(version);
   
   /* 
    * enum information is only present if the target is an enum. If not
    * present, we use "#define" instead.
    */
   if ( commonArgs->writingEnum == 1 ) {
      fprintf( commonArgs->fpHeader, "enum %s\n{\n", commonArgs->enumname );
   }
   
   if ( commonArgs->using_cs ) {
      if ( commonArgs->cs_namespace != NULL ) {
         fprintf( commonArgs->fpCS, "namespace %s\n{\n", commonArgs->cs_namespace );
         fprintf( commonArgs->fpCS, "    public enum %s\n    {\n", commonArgs->cs_enum );
      }
      else {
         fprintf( commonArgs->fpCS, "public enum %s\n{\n", commonArgs->cs_enum );
      }
   }
   if ( commonArgs->fpPyH ) {
      fprintf( commonArgs->fpPyH, "PyObject * AddErrors(void)\n{\n" );
      fprintf( commonArgs->fpPyH, "    PyObject * errors = NULL, * value = NULL;\n" );
      fprintf( commonArgs->fpPyH, "    int errcode;\n\n" );
      fprintf( commonArgs->fpPyH, "    errors = PyDict_New();\n" );
      fprintf( commonArgs->fpPyH, "    if ( errors == NULL ) return NULL;\n\n" );
   }
   if ( commonArgs->fpPyPy ) {
      fprintf( commonArgs->fpPyPy, "errs = dict()\n\n" );
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
         if ( group != NULL ) addGroup( commonArgs, group, 0 );
         group = node;
      }
      node = node->next; 
   }
   addGroup( commonArgs, group, 1 );

   if ( commonArgs->writingEnum == 1 ) {
      fprintf( commonArgs->fpHeader, "};\n\n" );
      fprintf( commonArgs->fpHeader, "typedef enum %s %s;\n\n", 
         commonArgs->enumname, commonArgs->enumname );
   }

   if ( commonArgs->using_cs ) {
      if ( commonArgs->cs_namespace != NULL ) {
         fprintf( commonArgs->fpCS, "    }\n}\n\n" );
      }
      else {
         fprintf( commonArgs->fpCS, "}\n\n" );
      }
   }
   if ( commonArgs->fpPyH ) {
      fprintf( commonArgs->fpPyH, "    return errors;\n}\n\n" );
   }

   writeMsgHeader( commonArgs );
   writeErrorMessage( commonArgs );

   stopHeaderGuard( (char*)commonArgs->headerName, commonArgs->fpHeader );
   stopHeaderGuard( (char*)commonArgs->outputNameH, commonArgs->fpMsgH );
   if ( commonArgs->fpPyH ) {
      stopHeaderGuard( (char*)commonArgs->py_filename, commonArgs->fpPyH );
   }
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

int main( int argc, char * argv[] )
{
   xmlParserCtxtPtr context = NULL;  /* The parser context */
   xmlNode * root = NULL;            /* The root node */
   int rc;
   struct errp_common commonArgs;
   char filename[PATH_MAX];
   FILE * fp_dummy;
   
   /*
    * this initialize the library and check potential ABI mismatches
    * between the version it was compiled for and the actual shared
    * library used.
    */
   LIBXML_TEST_VERSION

   memset( &commonArgs, 0, sizeof(struct errp_common) );
   
   rc = handleOptions( &commonArgs, argc, argv );
   if ( rc != 0 ) {
      if ( rc == 1 ) return 0; /* help */
      return 1;
   }
   
   buildPath( filename, commonArgs.headerDir, commonArgs.headerName );
   commonArgs.fpHeader = fopen( filename, "w" );
   if ( commonArgs.fpHeader == NULL ) {
      fprintf( stderr, "Error opening the header file %s.\n", filename );
      return 1;
   }
   buildPath( filename, commonArgs.outputDir, commonArgs.outputNameH );
   commonArgs.fpMsgH = fopen( filename, "w" );
   if ( commonArgs.fpMsgH == NULL ) {
      fprintf( stderr, "Error opening the error message header file %s.\n", 
         filename );
      return 1;
   }
   buildPath( filename, commonArgs.outputDir, commonArgs.outputNameC );
   commonArgs.fpMsgC = fopen( filename, "w" );
   if ( commonArgs.fpMsgC == NULL ) {
      fprintf( stderr, "Error opening the error message C file %s.\n", 
         filename );
      return 1;
   }

   if ( commonArgs.using_cs ) {
      commonArgs.fpCS = fopen( (char *)commonArgs.cs_filename, "w" );
      if ( commonArgs.fpCS == NULL ) {
         fprintf( stderr, "Error opening the C# file %s.\n", 
            commonArgs.cs_filename );
         return 1;
      }
   }

   if ( commonArgs.using_py ) {
      buildPath( filename, commonArgs.py_dirname, commonArgs.py_filename );
      fp_dummy = fopen( filename, "w" );
      if ( fp_dummy == NULL ) {
         fprintf( stderr, "Error opening the Python file %s.\n", 
            filename );
         return 1;
      }
      if ( commonArgs.using_py_extended ) commonArgs.fpPyH = fp_dummy;
      else commonArgs.fpPyPy = fp_dummy;
   }
   
   context = xmlNewParserCtxt();
   if ( context == NULL ) {
      fprintf(stderr, "Error allocating the parser context\n");
      return 1;
   }

   /* We create the document and validate in one go */
   commonArgs.document = xmlCtxtReadFile( context, 
                                          (char*)commonArgs.xmlFileName, 
                                          NULL,
                                          XML_PARSE_DTDVALID );
   if ( commonArgs.document == NULL ) {
      fprintf( stderr, "Error while parsing the input file: %s\n", 
               commonArgs.xmlFileName );
      return 1;
   }
   if ( context->valid == 0 ) {
      fprintf( stderr, "Error: document validation failed\n" );
      return 1;
   }
   
   root = xmlDocGetRootElement( commonArgs.document );

   navigate( &commonArgs, root );

   xmlFreeDoc( commonArgs.document );

   xmlFreeParserCtxt( context );

   xmlCleanupParser();

   fclose( commonArgs.fpHeader );
   fclose( commonArgs.fpMsgC );
   fclose( commonArgs.fpMsgH );
   if (commonArgs.fpCS) fclose( commonArgs.fpCS );
   if (commonArgs.fpPyH) fclose( commonArgs.fpPyH );
   if (commonArgs.fpPyPy) fclose( commonArgs.fpPyPy );

   if ( commonArgs.prefix != NULL ) free(commonArgs.prefix);
      
   return 0;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

