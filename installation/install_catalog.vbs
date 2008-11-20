' ***********************************************************************
'
' Copyright (C) 2008 Daniel Prevost <dprevost@users.sourceforge.net>
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
' Declaration of variables
'
' ***********************************************************************

Dim exeName
Dim errpCatalog
Dim topCatalog

Dim rc, status
Dim objShell
Dim objWshScriptExec
Dim fso

'$THE_PROGRAM --noout --add delegatePublic "-//Error Parser project//DTD Error Parser XML V1.1" $THE_ERRP_CATALOG $THE_TOP_CATALOG
'$THE_PROGRAM --noout --add delegateSystem "http://photonsoftware.org/xml/ep/1.1/" $THE_ERRP_CATALOG $THE_TOP_CATALOG

'Dim exe_name, prog_path, program, wd_path, tmpDir, cmdFile, exeName
Dim consoleMode
Dim cmd
Dim strOutput

' ***********************************************************************
'
' Initialization of these variables
'
' Please fix/set these variables to fit your installation.
'
' The Error Parser catalog and the DTD must be install in the same 
' directory.
'
' ***********************************************************************

exeName = """c:\Program Files\libxml2\bin\xmlcatalog.exe"""
topCatalog = """e:\main_catalog.xml"""

'The string for the Error Parser catalog uses '/' instead of '\'. That's
' because the xml top catalog expects this syntax. Don't change it.
errpCatalog = """c:/Program Files/Error Parser/DTD/catalog.xml"""
'errpCatalog = """e:/errorParser/DTD/catalog.xml"""

' Create the FileSystemObject
Set fso = CreateObject ("Scripting.FileSystemObject")

consoleMode = True
If Right(LCase(Wscript.FullName), 11) = "wscript.exe" Then
   consoleMode = False
End If

Set objShell = CreateObject("WScript.Shell")

' ***********************************************************************
'
' Start the program
'
' ***********************************************************************

if fso.FileExists(topCatalog) = false then
   cmd = exeName & " --noout --create " & topCatalog
   if consoleMode then 
      WScript.Echo "Creating the top catalog..."
      Set objWshScriptExec = objShell.Exec("%comspec% /c " & Chr(34) & cmd & Chr(34))
      status = objWshScriptExec.Status
      Do While objWshScriptExec.Status = 0
         WScript.Sleep 100
      Loop
      strOutput = objWshScriptExec.StdOut.ReadAll
      WScript.Stdout.Write objWshScriptExec.StdErr.ReadAll
      rc = objWshScriptExec.ExitCode
   else
      rc = objShell.Run("%comspec% /c " & Chr(34) & cmd & Chr(34), 2, true)
   end if
   if rc <> 0 then   
      wscript.echo "Failed with rc = " & rc & ": " & strOutput
      wscript.quit(1)
   end if
end if

cmd = exeName & " --noout --add delegatePublic ""-//Error Parser project//DTD Error Parser XML V1.2"" " & " " & errpCatalog & " " & topCatalog
if consoleMode then 
   WScript.Echo "Inserting the delegatePublic to the top catalog..."
   Set objWshScriptExec = objShell.Exec("%comspec% /c " & Chr(34) & cmd & Chr(34))
   status = objWshScriptExec.Status
   Do While objWshScriptExec.Status = 0
      WScript.Sleep 100
   Loop
   strOutput = objWshScriptExec.StdOut.ReadAll
   WScript.Stdout.Write objWshScriptExec.StdErr.ReadAll
   rc = objWshScriptExec.ExitCode
else
   rc = objShell.Run("%comspec% /c " & Chr(34) & cmd & Chr(34), 2, true)
end if
if rc <> 0 then   
   wscript.echo "Failed with rc = " & rc & ": " & strOutput
   wscript.quit(1)
end if

cmd = exeName & " --noout --add delegatePublic ""-//Error Parser project//DTD Error Parser Options XML V1.1"" " & " " & errpCatalog & " " & topCatalog
if consoleMode then 
   WScript.Echo "Inserting the second delegatePublic to the top catalog..."
   Set objWshScriptExec = objShell.Exec("%comspec% /c " & Chr(34) & cmd & Chr(34))
   status = objWshScriptExec.Status
   Do While objWshScriptExec.Status = 0
      WScript.Sleep 100
   Loop
   strOutput = objWshScriptExec.StdOut.ReadAll
   WScript.Stdout.Write objWshScriptExec.StdErr.ReadAll
   rc = objWshScriptExec.ExitCode
else
   rc = objShell.Run("%comspec% /c " & Chr(34) & cmd & Chr(34), 2, true)
end if
if rc <> 0 then   
   wscript.echo "Failed with rc = " & rc & ": " & strOutput
   wscript.quit(1)
end if
cmd = exeName & " --noout --add delegatePublic ""-//Error Parser project//DTD Error Parser Options XML V1.0"" " & " " & errpCatalog & " " & topCatalog
if consoleMode then 
   WScript.Echo "Inserting the third delegatePublic to the top catalog..."
   Set objWshScriptExec = objShell.Exec("%comspec% /c " & Chr(34) & cmd & Chr(34))
   status = objWshScriptExec.Status
   Do While objWshScriptExec.Status = 0
      WScript.Sleep 100
   Loop
   strOutput = objWshScriptExec.StdOut.ReadAll
   WScript.Stdout.Write objWshScriptExec.StdErr.ReadAll
   rc = objWshScriptExec.ExitCode
else
   rc = objShell.Run("%comspec% /c " & Chr(34) & cmd & Chr(34), 2, true)
end if
if rc <> 0 then   
   wscript.echo "Failed with rc = " & rc & ": " & strOutput
   wscript.quit(1)
end if

cmd = exeName & " --noout --add delegateSystem ""http://photonsoftware.org/xml/ep/"" " & " " & errpCatalog & " " & topCatalog
if consoleMode then 
   WScript.Echo "Inserting the delegateSystem to the top catalog..."
   Set objWshScriptExec = objShell.Exec("%comspec% /c " & Chr(34) & cmd & Chr(34))
   status = objWshScriptExec.Status
   Do While objWshScriptExec.Status = 0
      WScript.Sleep 100
   Loop
   strOutput = objWshScriptExec.StdOut.ReadAll
   WScript.Stdout.Write objWshScriptExec.StdErr.ReadAll
   rc = objWshScriptExec.ExitCode
else
   rc = objShell.Run("%comspec% /c " & Chr(34) & cmd & Chr(34), 2, true)
end if
if rc <> 0 then   
   wscript.echo "Failed with rc = " & rc & ": " & strOutput
   wscript.quit(1)
end if

wscript.echo "If not done already, you must set the environment variable"
wscript.echo "XML_CATALOG_FILES to the location of the top catalog."

wscript.quit(0)

