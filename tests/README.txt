All the tests uses errorParser (evidently). 

The current set of tests always produced output files which are then 
compared with "known good files". If identical, the tests pass.

There are no tests to check the syntax of the xml files themselves - the 
validation of the syntax done by the parser of libxml2 is presumed to be 
working correctly (it would be possible to add tests for this if problems 
are encountered).

--------------------------------------------------

This file was last updated on Jan 03, 2009 in preparation for release 1.0
RC 1.

--------------------------------------------------

Directories:

options
  This directory is for the xml files used to store options for errorParser.

xml
  This directory is used for the input xml files (containing the definitions
  of errors for the tests).
  
baseline
   This directory contains the baseline output files that will be compared
   to the files by the tests.

   For tests using the "generic" option file, the baseline files are in
   subdirectories (one for each test) since they all have the same names.

junk
   Directory created on the fly (if absent) to put temporary output files. 
   If an error occurs, the temporary files are not removed and they can 
   be used to debug the tests or the program.

--------------------------------------------------

Description of the tests:

XML Error Tests:

These tests make sure that Error Parser does the "right thing" when
interpreting the xml error file.

french
   this test makes sure that the parser pick the proper message/description
   when multiple messages/descriptions are available.

multi_copyright  
   the xml file has multiple copyright statements. They should all appear in
   the output files.

no_copyright 
   there is no copyright information in the xml file. Since this field is
   optional, this should be ok.

no_groupdesc 
   the <groupdescript> is missing for some <errgroup>. Since this field is
   optional, this should be ok.

no_groupident
   the <groupident> is missing for some <errgroup>. Since this field is
   optional, this should be ok.

no_version 
   the version attribute is missing from <errorlist>. Since this field is
   optional, this should be ok.

one_error 
   a single error. Should be ok (getError.c is the possible critical point
   since it deals with an array of pointers with a single element).

two_english
   two <message xml:lang="en">. The first one should be used and the second 
   one should be ignored.

--------------------------------------------------

XML Options Tests:

These tests validate the output files by type (and tests the options available
for each type of output).

csharp
   validates the C# output

csharp_nons
   validates the C# output when no namespace is given

errmsg
   validates both error messages output files

errmsg_no_dir
   validates both error messages output files when <errmsg_dirname> is absent.

ext_py
   validates the python-extension header file.

ext_py_no_dir
   validates the python-extension header file when <ext_py_dirname> is absent.

ext_py_no_func
   validates the python-extension header file when <ext_py_function> is absent.

header
   validates the main header file.

header_no_dir
   validates the main header file when <header_dirname> is absent.

header_no_enum
   validates the main header file when #define are used instead of an enum.

pure_python
   validates the pure python output file.

pure_python_no_func
   validates the pure python output file when <pure_py_function> is absent.


