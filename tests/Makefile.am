#############################################################################
#                                                                           #
# Copyright (C) 2008 Daniel Prevost <dprevost@users.sourceforge.net>        #
#                                                                           #
# This file may be distributed and/or modified under the terms of the       #
# MIT License as described by the Open Source Initiative                    #
# (http://opensource.org/licenses/mit-license.php) and appearing in         #
# the file COPYING included in the packaging of this software.              #
#                                                                           #
# This program is distributed in the hope that it will be useful, but       #
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the    #
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  #
#                                                                           #
#############################################################################

# Warning: header files will be removed by make clean!
CLEANFILES = *~ core *.h

EXTRA_DIST =                              \
        baseline/no_copyright/errors.h    \
        baseline/no_copyright/getError.c  \
        baseline/no_copyright/getError.h  \
        baseline/no_groupident/errors.h   \
        baseline/no_groupident/getError.c \
        baseline/no_groupident/getError.h \
        baseline/one_error/errors.h       \
        baseline/one_error/getError.c     \
        baseline/one_error/getError.h     \
        options/no_copyright.xml          \
        options/no_groupident.xml         \
        options/one_error.xml             \
        xml/no_copyright.xml              \
        xml/no_groupident.xml             \
        xml/one_error.xml                 \
        README.txt                        \
        RunTests.vbs                      \
        Tests.sh                          

TESTS_ENVIRONMENT =                  \
        tcp_port=10701               \
        top_srcdir=$(top_srcdir)     \
        srcdir=$(srcdir)             \
        top_builddir=$(top_builddir) \
        verbose=1                    

TESTS =  Tests.sh

