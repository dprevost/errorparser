' ***********************************************************************
'
' Copyright (C) 2008-2009 Daniel Prevost <dprevost@users.sourceforge.net>
'
' This file may be distributed and/or modified under the terms of the
' MIT License as described by the Open Source Initiative
' (http://opensource.org/licenses/mit-license.php) and appearing in
' the file COPYING included in the packaging of this software.
'
' This program is distributed in the hope that it will be useful, but
' WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
' implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
'
' ***********************************************************************

Option Explicit

' ***********************************************************************
'
' Function to run a program
'
' ***********************************************************************

Function runProg( prog, consoleMode, verbose )

   Dim objWshScriptExec, status, rc, strOutput

   if consoleMode then 
      Set objWshScriptExec = objShell.Exec("%comspec% /c " & Chr(34) & prog & Chr(34))
      status = objWshScriptExec.Status
      Do While objWshScriptExec.Status = 0
         WScript.Sleep 100
      Loop
      strOutput = objWshScriptExec.StdOut.ReadAll
      if verbose then 
         WScript.Stdout.Write objWshScriptExec.StdErr.ReadAll
      end if
      rc = objWshScriptExec.ExitCode
   else
      rc = objShell.Run("%comspec% /c " & Chr(34) & prog & Chr(34), 2, true)
   end if
   
   runProg = rc
   
End Function

' ***********************************************************************
'
' Declaration of variables
'
' ***********************************************************************

Dim rc, numTests, numFailed, verbose
Dim objShell
Dim fso

' List of failed tests. We append to this list when an error is encountered
' while running the tests
Dim failed_tests(18)

' Lists containing the names of the tests
' The "error" list is for tests of the xml error file itself (as in, no
' copyright section, multiple copyright paragraphs, etc.).
Dim error_tests(6)

' The "option" list is for tests of the xml option file (as in, C#, 
' C# without a namespace, etc.
Dim option_tests(11)
Dim option_file(11)
Dim option_file2(11)

' Path to the GNU utility diff.exe
' A version of this file (from http://sourceforge.net/projects/unxutils)
' is in the tests directory.
Dim diff_path
Dim diff_exe
Dim diff_options

Dim exe_name, prog_path
Dim consoleMode
Dim objArgs, strArg, i
Dim test, xml_test, test_exec

' ***********************************************************************
'
' Initialization of these variables
'
' ***********************************************************************

' Populate the test lists...
error_tests(0) = "french" 
error_tests(1) = "multi_copyright"
error_tests(2) = "no_copyright"
error_tests(3) = "no_groupident"
error_tests(4) = "no_version"
error_tests(5) = "one_error"
error_tests(6) = "two_english"

option_tests(0)  = "csharp"
option_file (0)  = "csharp.cs"
option_file2(0)  = ""
option_tests(1)  = "csharp_nons"
option_file (1)  = "csharp_nons.cs"
option_file2(1)  = ""
option_tests(2)  = "errmsg"
option_file (2)  = "errmsg.h"
option_file2(2)  = "errmsg.c"
option_tests(3)  = "errmsg_no_dir"
option_file (3)  = "errmsg_no_dir.h"
option_file2(3)  = "errmsg_no_dir.c"
option_tests(4)  = "ext_py"
option_file (4)  = "ext_py.h"
option_file2(4)  = ""
option_tests(5)  = "ext_py_no_dir"
option_file (5)  = "ext_py_no_dir.h"
option_file2(5)  = ""
option_tests(6)  = "ext_py_no_func"
option_file (6)  = "ext_py_no_func.h"
option_file2(6)  = ""
option_tests(7)  = "header"
option_file (7)  = "header.h"
option_file2(7)  = ""
option_tests(8)  = "header_no_dir"
option_file (8)  = "header_no_dir.h"
option_file2(8)  = ""
option_tests(9)  = "header_no_enum"
option_file (9)  = "header_no_enum.h"
option_file2(9)  = ""
option_tests(10) = "pure_python"
option_file (10) = "pure_python.py"
option_file2(10) = ""
option_tests(11) = "pure_python_no_func"
option_file (11) = "pure_python_no_func.py"
option_file2(11) = ""

numTests = 19
numFailed = 0

' Create the FileSystemObject
Set fso = CreateObject ("Scripting.FileSystemObject")

consoleMode = True
If Right(LCase(Wscript.FullName), 11) = "wscript.exe" Then
   consoleMode = False
End If

Set objShell = CreateObject("WScript.Shell")
verbose = True

prog_path = "..\parser\Project_Files\Release"
diff_path = "."
diff_options = " -b -I "" * Date: "" -I ""using the input file"" "
'diff_options = " -b -I ' * Date: ' -I 'using the input file' "

Set objArgs = WScript.Arguments

' ***********************************************************************
'
' Start the program
'
' ***********************************************************************

For Each strArg in objArgs
   If Left(LCase(strArg), 6) = "--path" AND Len(strArg) > 6 Then
      prog_path = Right(strArg, Len(strArg)-7)
   end if
Next 

if Not consoleMode then
   wscript.echo "Be patient - running the tests in batch mode - click ok to start"
end if

'Turn off error handling for the creation of "junk". 
On Error Resume Next
fso.CreateFolder("junk")
On Error GoTo 0 

exe_name = prog_path & "\errorParser.exe"
diff_exe = diff_path & "\diff.exe"

' Run all error tests
For Each test in error_tests
   xml_test = test & ".xml"
   test_exec = exe_name & " --options options\generic.xml" & " xml\" & xml_test
   if consoleMode then 
      WScript.Echo "Running " & test
   end if

   rc = runProg(test_exec, consoleMode, verbose)
   if rc <> 0 then   
      failed_tests(numFailed) = test
      numFailed = numFailed + 1
   else
      test_exec = diff_exe & diff_options & " junk\errors.h baseline\" & test & "\errors.h"
      rc = runProg(test_exec, consoleMode, verbose)
      if rc <> 0 then   
         failed_tests(numFailed) = test
         numFailed = numFailed + 1
      else
         test_exec = diff_exe & diff_options & " junk\getError.c baseline\" & test & "\getError.c"
         rc = runProg(test_exec, consoleMode, verbose)
         if rc <> 0 then   
            failed_tests(numFailed) = test
            numFailed = numFailed + 1
         else
            test_exec = diff_exe & diff_options & " junk\getError.h baseline\" & test & "\getError.h"
            rc = runProg(test_exec, consoleMode, verbose)
            if rc <> 0 then   
               failed_tests(numFailed) = test
               numFailed = numFailed + 1
            end if
         end if
      end if
   end if

   'Turn off error handling for the deletions of all "junk files"
   if rc = 0 then
      On Error Resume Next
      fso.DeleteFile("junk\errors.h")
      fso.DeleteFile("junk\getError.c")
      fso.DeleteFile("junk\getError.h")
      On Error GoTo 0 
   end if
Next

' Run all option tests
For i = 0 to 11 
   xml_test = option_tests(i) & ".xml"
   test_exec = exe_name & " --options options\" & xml_test & " xml\generic.xml" 
   if consoleMode then 
      WScript.Echo "Running " & option_tests(i)
   end if

   rc = runProg(test_exec, consoleMode, verbose)
   if rc <> 0 then   
      failed_tests(numFailed) = test
      numFailed = numFailed + 1
   else
      test_exec = diff_exe & diff_options & " junk\" & option_file(i) & " baseline\" & option_file(i)
      rc = runProg(test_exec, consoleMode, verbose)
      if rc <> 0 then   
         failed_tests(numFailed) = test
         numFailed = numFailed + 1
      else
         if option_file2(i) <> "" then
            test_exec = diff_exe & diff_options & " junk\" & option_file2(i) & " baseline\" & option_file2(i)
            rc = runProg(test_exec, consoleMode, verbose)
            if rc <> 0 then   
               failed_tests(numFailed) = test
               numFailed = numFailed + 1
            end if
         end if
      end if
   end if
Next

if consoleMode then
   WScript.StdOut.Write vbcrlf & "Total number of tests: " & numTests & vbcrlf
   WScript.StdOut.Write "Total number of failed tests: " & numFailed & vbcrlf & vbcrlf
   For i = 1 to numFailed 
      WScript.StdOut.Write "This test failed: " & failed_tests(i-1) & vbcrlf
   Next
   if verbose then
      WScript.StdOut.Write "Type return to continue"
      Dim dummy
      dummy = WScript.StdIn.Read(1)
   end if
else                                 
   wscript.echo "Total number of tests: " & numTests & vbcrlf & _
      "Total number of failed tests: " & numFailed
end if

if numFailed > 0 then wscript.quit(1)
wscript.quit(0)

