/*
 * This file was generated by the program errorParser
 * using the input file ../tests/xml/multi_copyright.xml.
 * Date: Tue Dec 30 10:00:36 2008.
 *
 * The version of the XML error file is 0.1.
 *
 * Copyright (C) 2008 D. Prevost
 *
 * This file is both a test and an example.
 *
 * This file may be distributed and/or modified under the terms of the 
 * MIT License as described by the Open Source Initiative 
 * (http://opensource.org/licenses/mit-license.php) and appearing in 
 * the file COPYING included in the packaging of this software.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.
 *
 * Copyright (C) 2007 D. Prevost #2
 *
 * This file may be distributed and/or modified under the terms of the 
 * MIT License as described by the Open Source Initiative 
 * (http://opensource.org/licenses/mit-license.php) and appearing in 
 * the file COPYING included in the packaging of this software.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.
 *
 * Copyright (C) 2006 D. Prevost #3
 *
 * This file may be distributed and/or modified under the terms of the 
 * MIT License as described by the Open Source Initiative 
 * (http://opensource.org/licenses/mit-license.php) and appearing in 
 * the file COPYING included in the packaging of this software.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.
 *
 */

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

#include <stdlib.h> /* Needed for NULL. */

#define BULDING_ERROR_MESSAGE
#include "getError.h"

struct prog_MsgStruct
{
    int  errorNumber;
    const char * message;
};

typedef struct prog_MsgStruct prog_MsgStruct;

/* MY_PROG_OK */
prog_MsgStruct prog_Msg0 = {
    0, "No error" };

/* MY_PROG_INTERNAL_ERROR */
prog_MsgStruct prog_Msg1 = {
    1, "Abnormal internal error - it should not happen!" };

/* MY_PROG_ENGINE_BUSY */
prog_MsgStruct prog_Msg2 = {
    2, "Cannot get a lock on a system object, the engine is \"busy\"" };

/* MY_PROG_SOCKET_ERROR */
prog_MsgStruct prog_Msg3 = {
    25, "Generic socket error." };

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

/* Array of pointers to previous structs */
prog_MsgStruct * prog_MsgArray[4] = {
    &prog_Msg0,
    &prog_Msg1,
    &prog_Msg2,
    &prog_Msg3
};

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

const char * prog_ErrorMessage( int errnum )
{
    int i;

    for ( i = 0; i < 4; ++i ) {
        if ( errnum == prog_MsgArray[i]->errorNumber ) {
            return prog_MsgArray[i]->message;
        }
    }

    return NULL;
}

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

