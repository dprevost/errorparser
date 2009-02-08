#############################################################################
#                                                                           #
# Copyright (C) 2008-2009 Daniel Prevost <dprevost@users.sourceforge.net>   #
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

CLEANFILES = *~ core 

EXTRA_DIST =            \
        baseline        \
        diff.exe        \
        Makefile.vs2005 \
        Makefile.vs2008 \
        options         \
        README.txt      \
        RunTests.vbs    \
        Tests.sh        \
        xml             

TESTS_ENVIRONMENT =                  \
        tcp_port=10701               \
        top_srcdir=$(top_srcdir)     \
        srcdir=$(srcdir)             \
        top_builddir=$(top_builddir) \
        verbose=1                    

TESTS =  Tests.sh

dist-hook:
	-rm -rf `find $(distdir) -name .svn`
	-rm -rf `find $(distdir) -name '*~'`

clean-local:
	-rm -rf `find . -name '*~'`
	-rm -rf junk
   
