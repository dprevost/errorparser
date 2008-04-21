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

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

void writeErrorMessage( errp_common * commonArgs )
{
   int i;
   
   fprintf( commonArgs->fpMsgC, "%s\n\n", g_barrier );
   /* Define an array of pointers to the previous structs */
   fprintf( commonArgs->fpMsgC, "/* Array of pointers to previous structs */\n" );
   fprintf( commonArgs->fpMsgC, "%s_MsgStruct * %s_MsgArray[%d] = {\n", 
      commonArgs->varPrefix, commonArgs->varPrefix, commonArgs->errorCount );

   for ( i = 0; i < commonArgs->errorCount-1; ++i ) {
      fprintf( commonArgs->fpMsgC, "    &%s_Msg%d,\n", 
         commonArgs->varPrefix, i );
   }
   fprintf( commonArgs->fpMsgC, "    &%s_Msg%d\n};\n\n", 
      commonArgs->varPrefix, commonArgs->errorCount-1 );

   fprintf( commonArgs->fpMsgC, "%s\n\n", g_barrier );

   /* Write the function to access the error messages */
   fprintf( commonArgs->fpMsgC, "const char * %s_ErrorMessage( int errnum )\n",
      commonArgs->varPrefix );
   fprintf( commonArgs->fpMsgC, "{\n" );
   fprintf( commonArgs->fpMsgC, "    int i;\n\n" );

   fprintf( commonArgs->fpMsgC, "    for ( i = 0; i < %d; ++i ) {\n", 
      commonArgs->errorCount );
   fprintf( commonArgs->fpMsgC, "        if ( errnum == %s_MsgArray[i]->errorNumber ) {\n", 
      commonArgs->varPrefix );
   fprintf( commonArgs->fpMsgC, "            return %s_MsgArray[i]->message;\n",
      commonArgs->varPrefix );
   fprintf( commonArgs->fpMsgC, "        }\n" );
   fprintf( commonArgs->fpMsgC, "    }\n\n" );

   fprintf( commonArgs->fpMsgC, "    return NULL;\n" );
   fprintf( commonArgs->fpMsgC, "}\n\n" );

   fprintf( commonArgs->fpMsgC, "%s\n\n", g_barrier );
}


/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

