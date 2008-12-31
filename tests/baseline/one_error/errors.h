/*
 * This file was generated by the program errorParser
 * using the input file ../tests/xml/one_error.xml.
 * Date: Tue Dec 30 10:05:47 2008.
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
    MY_PROG_OK = 0
};

typedef enum enumErrors enumErrors;

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

#ifdef __cplusplus
}
#endif

#endif /* ERRORS_H */

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

