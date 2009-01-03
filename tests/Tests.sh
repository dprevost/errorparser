#!/bin/sh

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

run_test_xml()
{
   test_name=$1

   if [ $verbose = 1 ] ; then
      $parser --options $test_dir/options/generic.xml $test_dir/xml/$test_name.xml
   else
      $parser --options $test_dir/options/generic.xml $test_dir/xml/$test_name.xml >/dev/null 2>&1
   fi
   if [ "$?" != 0 ] ; then
      echo "FAIL (errorParser): $test_name "
      return 1
   fi

   diff -b -I ' * Date: ' -I 'using the input file' junk/errors.h $test_dir/baseline/$test_name/errors.h
   if [ "$?" != 0 ] ; then
      echo "FAIL (diff): $test_name "
      return 1
   fi
   diff -b -I ' * Date: ' -I 'using the input file' junk/getError.c $test_dir/baseline/$test_name/getError.c
   if [ "$?" != 0 ] ; then
      echo "FAIL (diff): $test_name "
      return 1
   fi
   diff -b -I ' * Date: ' -I 'using the input file' junk/getError.h $test_dir/baseline/$test_name/getError.h
   if [ "$?" != 0 ] ; then
      echo "FAIL (diff): $test_name "
      return 1
   fi

   if [ $verbose = 1 ] ; then
      echo "PASS: $test_name "
   fi

   rm junk/errors.h 
   rm junk/getError.c
   rm junk/getError.h
}

# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
# 
# This function takes multiple parameters:
#  1 - the name of the test and option file (same name for both xml file 
#      minus the .xml extension)
#  2 - the name of the file to "diff" with
#
# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

run_more_test()
{
   test_name=$1
   test_file=$2
   
   if [ $# = 3 ] ; then
      test_file2=$3
   fi
   
   if [ $verbose = 1 ] ; then
      $parser --options $test_dir/options/$test_name.xml $test_dir/xml/generic.xml
   else
      $parser --options $test_dir/options/$test_name.xml $test_dir/xml/generic.xml >/dev/null 2>&1
   fi
   if [ "$?" != 0 ] ; then
      echo "FAIL (errorParser): $test_name "
      return 1
   fi

   diff -b -I 'Date: ' -I 'using the input file' junk/$test_file $test_dir/baseline/$test_file
   if [ "$?" != 0 ] ; then
      echo "FAIL (diff): $test_name "
      return 1
   fi
   if [ $# = 3 ] ; then
      diff -b -I 'Date: ' -I 'using the input file' junk/$test_file2 $test_dir/baseline/$test_file2
      if [ "$?" != 0 ] ; then
         echo "FAIL (diff): $test_name "
         return 1
      fi
   fi
   
   if [ $verbose = 1 ] ; then
      echo "PASS: $test_name "
   fi

   rm junk/$test_file
   if [ $# = 3 ] ; then
      rm junk/$test_file2
   fi
}

# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

rm -rf junk
mkdir junk

good_tests="french multi_copyright no_copyright no_groupident \
no_version one_error two_english"

echo "Running tests for the xml error file itself"
for tests in $good_tests; do
   run_test_xml $tests
   if [ "$?" != 0 ] ; then
      exit 1
   fi
done

echo "Running tests for each possible output file (with its options)"

run_more_test csharp csharp.cs
if [ "$?" != 0 ] ; then
   exit 1
fi
run_more_test csharp_nons csharp_nons.cs
if [ "$?" != 0 ] ; then
   exit 1
fi

run_more_test errmsg errmsg.h errmsg.c
if [ "$?" != 0 ] ; then
   exit 1
fi
run_more_test errmsg_no_dir errmsg_no_dir.h errmsg_no_dir.c
if [ "$?" != 0 ] ; then
   exit 1
fi

run_more_test ext_py ext_py.h
if [ "$?" != 0 ] ; then
   exit 1
fi
run_more_test ext_py_no_dir ext_py_no_dir.h
if [ "$?" != 0 ] ; then
   exit 1
fi
run_more_test ext_py_no_func ext_py_no_func.h
if [ "$?" != 0 ] ; then
   exit 1
fi

run_more_test header header.h
if [ "$?" != 0 ] ; then
   exit 1
fi
run_more_test header_no_dir header_no_dir.h
if [ "$?" != 0 ] ; then
   exit 1
fi
run_more_test header_no_enum header_no_enum.h
if [ "$?" != 0 ] ; then
   exit 1
fi

run_more_test pure_python pure_python.py
if [ "$?" != 0 ] ; then
   exit 1
fi
run_more_test pure_python_no_func pure_python_no_func.py
if [ "$?" != 0 ] ; then
   exit 1
fi

exit 0

# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

rm -rf junk
exit 0

# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

