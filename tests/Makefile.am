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

EXTRA_DIST =                          \
        test_no_copyright.h.baseline  \
        test_no_copyright.xml         \
        test_no_groupident.h.baseline \
        test_no_groupident.xml        \
        test_one_error.h.baseline     \
        test_one_error.xml            \
        Tests.sh                      

#        Makefile.Win32 \
        RunTests.vbs   

TESTS_ENVIRONMENT =                  \
        tcp_port=10701               \
        top_srcdir=$(top_srcdir)     \
        srcdir=$(srcdir)             \
        top_builddir=$(top_builddir) \
        verbose=1                    

TESTS =  Tests.sh

