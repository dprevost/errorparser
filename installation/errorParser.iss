[Setup]
MinVersion=0,4.0
AppName=Error Parser
AppVerName=Error Parser Version 0.3.0
AppVersion=0.3.0
AppSupportURL=http://errorparser.sourceforge.net/
AppId=ERRP_V0
LicenseFile=COPYING
DefaultDirName={pf}\errorParser
DefaultGroupName=Error Parser
SourceDir=..
OutputDir=installation\Output
OutputBaseFilename=errorParser-0.3.0
ChangesEnvironment=yes

[Files]
Source: DTD\catalog12.xml; DestDir: {app}\DTD
Source: DTD\errorParser12.dtd; DestDir: {app}\DTD
Source: doc\errorParser.chm; DestDir: {app}
Source: examples\example_options.xml; DestDir: {app}\Examples
Source: examples\example_vdsf.xml; DestDir: {app}\Examples
Source: examples\simple_example.xml; DestDir: {app}\Examples
Source: parser\Project_Files\Release\errorParser.exe; DestDir: {app}\bin
Source: COPYING; DestDir: {app}
Source: installation\install_catalog.vbs; DestDir: {app}\DTD
Source: installation\SetEnv.exe; DestDir: {app}\bin
Source: C:\Program Files\libxml2\bin\iconv.dll; DestDir: {app}\bin; Components: libxml2
Source: C:\Program Files\libxml2\bin\iconv.exe; DestDir: {app}\bin; Components: libxml2
Source: C:\Program Files\libxml2\bin\libxml2.dll; DestDir: {app}\bin; Components: libxml2
Source: C:\Program Files\libxml2\bin\xmlcatalog.exe; DestDir: {app}\bin; Components: libxml2

[Icons]
Name: {group}\Error Parser Help; Filename: {app}\errorParser.chm
Name: {group}\Uninstall Error Parser; Filename: {uninstallexe}

[Registry]
Root: HKCU; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\errorParser.exe; ValueType: string; ValueData: {app}\bin\errorParser.exe; Flags: uninsdeletekeyifempty uninsdeletevalue
Root: HKCU; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\errorParser.exe; ValueType: string; ValueName: Path; ValueData: {app}\bin; Flags: uninsdeletekeyifempty uninsdeletevalue

Root: HKLM; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\errorParser.exe; ValueType: string; ValueData: {app}\bin\errorParser.exe; Flags: uninsdeletekeyifempty uninsdeletevalue
Root: HKLM; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\errorParser.exe; ValueType: string; ValueName: Path; ValueData: {app}\bin; Flags: uninsdeletekeyifempty uninsdeletevalue
[Tasks]
Name: setenv; Description: "Add Error Parser to your ""path"""
[Run]
Filename: {app}\bin\SetEnv.exe; Parameters: "-ua Path %""{app}\bin"""; Flags: runminimized; Tasks: setenv
[UninstallRun]
Filename: {app}\bin\SetEnv.exe; Parameters: "-ud Path %""{app}\bin"""; Flags: runminimized
[Components]
Name: libxml2; Description: The libxml2 library; Types: with_libxml2
[Types]
Name: with_libxml2; Description: Install everything including libxml2 (run-time)
Name: without_libxml2; Description: Install everything except libxml2
