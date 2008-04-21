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
   
   /* Define an array of pointers to the previous structs */
   fprintf( commonArgs->fpMsgC, "%s_MsgStruct * %s_MsgArray[%d] = {\n", 
      commonArgs->varPrefix, commonArgs->varPrefix, commonArgs->errorCount );

   for ( i = 0; i < commonArgs->errorCount-1; ++i ) {
      fprintf( commonArgs->fpMsgC, "    &%s_Msg%d,\n", 
         commonArgs->varPrefix, i );
   }
   fprintf( commonArgs->fpMsgC, "    &%s_Msg%d\n};\n\n", 
      commonArgs->varPrefix, commonArgs->errorCount-1 );

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



}


/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

#if 0

vdseErrErrMsgStruct* vdseErrErrMsg[VDSEERR_MAX_ENTRIES] = 
{
   &vdseErrErrMsg0,
   &vdseErrErrMsg1,
   &vdseErrErrMsg2,
   &vdseErrErrMsg3,
   &vdseErrErrMsg4,
   &vdseErrErrMsg5,
   &vdseErrErrMsg6,
   &vdseErrErrMsg7,
   &vdseErrErrMsg8,
   &vdseErrErrMsg9,
   &vdseErrErrMsg10,
   &vdseErrErrMsg11,
   &vdseErrErrMsg12,
   &vdseErrErrMsg13,
   &vdseErrErrMsg14,
   &vdseErrErrMsg15,
   &vdseErrErrMsg16,
   &vdseErrErrMsg17,
   &vdseErrErrMsg18,
   &vdseErrErrMsg19,
   &vdseErrErrMsg20,
   &vdseErrErrMsg21,
   &vdseErrErrMsg22,
   &vdseErrErrMsg23,
   &vdseErrErrMsg24,
   &vdseErrErrMsg25,
   &vdseErrErrMsg26,
   &vdseErrErrMsg27,
   &vdseErrErrMsg28,
   &vdseErrErrMsg29,
   &vdseErrErrMsg30,
   &vdseErrErrMsg31,
   &vdseErrErrMsg32,
   &vdseErrErrMsg33,
   &vdseErrErrMsg34,
   &vdseErrErrMsg35,
   &vdseErrErrMsg36,
   &vdseErrErrMsg37,
   &vdseErrErrMsg38,
   &vdseErrErrMsg39,
   &vdseErrErrMsg40,
   &vdseErrErrMsg41

};

int vdseErrGetErrMessage( int errnum, char *msg, unsigned int msgLength )
{
   int i;

   if ( msgLength > 0 ) msg[0] = '\0';
   for ( i = 0; i < VDSEERR_MAX_ENTRIES; ++i )
   {
      if ( errnum == vdseErrErrMsg[i]->errorNumber )
      {
         if ( msgLength > 0 && vdseErrErrMsg[i]->messageLength > 0 )
         {
            strncpy( msg, vdseErrErrMsg[i]->message, msgLength-1 );
            msg[msgLength-1] = '\0';
         }
         return 0;
      }
   }

   return -1;
}
#endif
