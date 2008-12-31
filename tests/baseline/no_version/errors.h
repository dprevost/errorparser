/*
 * This file was generated by the program errorParser
 * using the input file ../tests/xml/no_version.xml.
 * Date: Tue Dec 30 10:04:43 2008.
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
 */

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

#ifndef ERRORS_H
#define ERRORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

enum enumErrors
{
    /*
     * A dummy group
     *
     * This group is useless. Just to test if this works
     */

    /**
     * No error
     */
    MY_PROG_OK = 0,

    /**
     * Abnormal internal error - it should not happen!
     */
    MY_PROG_INTERNAL_ERROR = 1,

    /**
     * Cannot get a lock on a system object, the engine is "busy"
     *
     * This might be the result of either a very busy system where 
     * unused cpu cycles are rare or a lock might be held by a crashed 
     * process.
     */
    MY_PROG_ENGINE_BUSY = 2,

    /*
     * Network errors
     *
     * The following errors are network related.
     */

    /**
     * Generic socket error.
     */
    MY_PROG_SOCKET_ERROR = 25
};

typedef enum enumErrors enumErrors;

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

#ifdef __cplusplus
}
#endif

#endif /* ERRORS_H */

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

