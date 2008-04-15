#!/bin/sh

#############################################################################
#                                                                           #
# Copyright (C) 2008 Daniel Prevost <dprevost@users.sourceforge.net>        #
#                                                                           #
# This file may be distributed and/or modified under the terms of the       #
# MIT License as described by the Open Source Initiative                    #
# (http://opensource.org/licenses/mit-license.php) and appearing in         #
# the file COPYING included in the packaging of this software.               #
#                                                                           #
# This program is distributed in the hope that it will be useful, but       #
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the    #
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  #
#                                                                           #
#############################################################################

# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
#
# To run these tests standalone (outside the scope of make check), you
# must provide some basic arguments:
#
#  - the path of the top directory of the source
#  - the path of the top directory of the build
#
# For example: 
# 
# ./Tests.sh /home/dprevost/errorParser /home/dprevost/errorParser
#
# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

# If the env. variable top_srcdir is not defined... we must have two
# arguments if we want to be able to run the test.
if test -z "$top_srcdir"; then
   if [ "$#" != 2 ] ; then
      echo "usage: $0 src_dir build_dir"
      exit 1
   fi

   top_srcdir=$1
   top_builddir=$2
   verbose=1
fi

# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

test_dir=$top_srcdir/tests
parser=$top_builddir/parser/errorParser

# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
# 
# This function take a single parameter, the name of the test (which is
# the name of the xml file minus the .xml extension).
#
# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

run_test()
{
   test_name=$1

   if [ $verbose = 1 ] ; then
      $parser --header $test_name.h $test_dir/$test_name.xml
   else
      $parser --header $test_name.h $test_dir/$test_name.xml >/dev/null 2>&1
   fi
   if [ "$?" != 0 ] ; then
      echo "FAIL (errorParser): $test_name "
      return 1
   fi

   diff -I ' * Date: ' -I 'using the input file' $test_name.h $test_dir/$test_name.h.baseline
   if [ "$?" != 0 ] ; then
      echo "FAIL (diff): $test_name "
      return 1
   fi

   if [ $verbose = 1 ] ; then
      echo "PASS: $test_name "
   fi

   rm $test_name.h
}

# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

run_test test_one_error
if [ "$?" != 0 ] ; then
   exit 1
fi
run_test test_no_copyright
if [ "$?" != 0 ] ; then
   exit 1
fi
run_test test_no_groupident
if [ "$?" != 0 ] ; then
   exit 1
fi

exit 0

# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

