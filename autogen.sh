#!/bin/sh
#
#############################################################################
#                                                                           #
# Copyright (C) 2008 Daniel Prevost <dprevost@users.sourceforge.net>        #
#                                                                           #
# This file may be distributed and/or modified under the terms of the       #
# MIT License as described by the Open Source Initiative                    #
# (http://opensource.org/licenses/mit-license.php) and appearing in         #
# the file COPYING included in the packaging of this library.               #
#                                                                           #
# This program is distributed in the hope that it will be useful, but       #
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the    #
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  #
#                                                                           #
#############################################################################

# Test our version of automake - we need at least 1.5 for nobase_ 
# although the code was always tested with 1.7 or greater. 
 
echo "Testing the version of automake."

am_version=`automake --version | head -1 | awk '{print $4}'`
am_major=`echo $am_version | awk -F . '{print $1}'`
am_minor=`echo $am_version | awk -F . '{print $2}'`

# The next one is needed because the format changed from somethink like 1.4-p6 
# to something like 1.7.8. 
am_minor=`echo $am_minor | awk -F - '{print $1}'`

#
if [ $am_major -lt 1 ] || [ $am_major -eq 1 -a $am_minor -lt 5 ] 
then
   echo "Error: automake 1.5 or later is required (your version is $am_version)."
   exit 1
fi

echo "Automake version: $am_version - all is ok"

##########################################################################

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

chmod 755 Config/config.guess
chmod 755 Config/config.sub
chmod 755 Config/depcomp
chmod 755 Config/install-sh
chmod 755 Config/ltmain.sh
chmod 755 Config/missing
chmod 755 Config/mkinstalldirs

echo "-----------------------------------------------"
echo "End of cleanup phase - warning messages are not"
echo "expected from now on and should be examined... "
echo "-----------------------------------------------"

##########################################################################

echo "- aclocal -I m4"
aclocal -I m4
echo "- autoconf"
autoconf
echo "- autoheader"
autoheader
echo "- automake"
automake
echo "- ./configure"
./configure
#echo "- make distcheck"
#make distcheck
exit
