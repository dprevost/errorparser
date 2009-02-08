#!/bin/sh
#
#############################################################################
#                                                                           #
# Copyright (C) 2008-2009 Daniel Prevost <dprevost@users.sourceforge.net>        #
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

## Some cleanup, just in case (probably overkill...).

echo "-----------------------------------------------"
echo "Cleanup phase - Warning messages are normal" 
echo "------------------------------------------------"

if [ -f Makefile ] 
then 
  make distclean
fi

rm -f config.cache
rm -f autom4te.cache/output*
rm -f autom4te.cache/requests*
rm -f autom4te.cache/traces*

echo "-----------------------------------------------"
echo "End of cleanup phase - warning messages are not"
echo "expected from now on and should be examined... "
echo "-----------------------------------------------"

##########################################################################

echo "- libtoolize -f"
libtoolize -f
echo "- aclocal -I m4"
aclocal -I m4
echo "- autoconf"
autoconf
echo "- autoheader"
autoheader
echo "- automake -a"
automake -a
echo "- ./configure"
./configure
exit
