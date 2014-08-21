# Microsoft Developer Studio Project File - Name="newray" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=newray - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "newray.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "newray.mak" CFG="newray - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "newray - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "newray - Win32 Profiling" (based on "Win32 (x86) Application")
!MESSAGE "newray - Win32 directX" (based on "Win32 (x86) Application")
!MESSAGE "newray - Win32 openPTC" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x80c /d "_DEBUG"
# ADD RSC /l 0x80c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib c:\openPTC\library\visual\ptcdebug.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "newray___Win32_Profiling"
# PROP BASE Intermediate_Dir "newray___Win32_Profiling"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "profiling"
# PROP Intermediate_Dir "profiling"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x80c /d "NDEBUG"
# ADD RSC /l 0x80c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib c:\openPTC\library\visual\ptc.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib c:\openPTC\library\visual\ptc.lib /nologo /subsystem:windows /profile /machine:I386

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "newray___Win32_directX"
# PROP BASE Intermediate_Dir "newray___Win32_directX"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "directX"
# PROP Intermediate_Dir "directX"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x80c /d "NDEBUG"
# ADD RSC /l 0x80c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib c:\openPTC\library\visual\ptc.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ddraw.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib c:\openPTC\library\visual\ptc.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "newray___Win32_openPTC"
# PROP BASE Intermediate_Dir "newray___Win32_openPTC"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "openPTC"
# PROP Intermediate_Dir "openPTC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"openPTC/newray.pch" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x80c /d "NDEBUG"
# ADD RSC /l 0x80c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib c:\openPTC\library\visual\ptc.lib /nologo /subsystem:windows /pdb:"openPTC/newray.pdb" /machine:I386 /out:"openPTC/newray.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib c:\openPTC\library\visual\ptc.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "newray - Win32 Debug"
# Name "newray - Win32 Profiling"
# Name "newray - Win32 directX"
# Name "newray - Win32 openPTC"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\math\cycloidal.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\cycloidal.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\math.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\matrix3.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\matrix3.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\matrix4.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\matrix4.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\solver.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\solver.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\transform.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\transform.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\vector2.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\vector2.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\vector3.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\vector3.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\vector4.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\math\vector4.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\math"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Profiling\math"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\math"
# PROP Intermediate_Dir "Release\math"

!ENDIF 

# End Source File
# End Group
# Begin Group "turbulences"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\turbulences\noise.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\turbulences"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\turbulences\noise.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\turbulences"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\turbulences\perlin.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\turbulences"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\turbulences\perlin.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\turbulences"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\turbulences\turbulence.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\turbulences"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\turbulences\turbulence.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\turbulences"

!ENDIF 

# End Source File
# End Group
# Begin Group "renderers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\bilinear.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\bilinear.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\corner.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\corner.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\diamond.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\diamond.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\flipquad.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\flipquad.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\grid.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\grid.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\interpolated.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\interpolated.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\quincunx.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\quincunx.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\renderer.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\renderer.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\rgss.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\renderers\rgss.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "profiling\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\renderers"
# PROP Intermediate_Dir "Release\renderers"

!ENDIF 

# End Source File
# End Group
# Begin Group "bitmap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\bitmap\bitmap.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Profiling\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Release\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Release\bitmap"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\bitmap\bitmap.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Profiling\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Release\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Release\bitmap"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\bitmap\bmp.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Profiling\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Release\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Release\bitmap"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\bitmap\gif.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\bitmap"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\bitmap\pcx.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Profiling\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Release\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Release\bitmap"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\bitmap\tga.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Profiling\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Release\bitmap"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\bitmap"
# PROP Intermediate_Dir "Release\bitmap"

!ENDIF 

# End Source File
# End Group
# Begin Group "lights"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\lights\dot.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Profiling\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Release\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Release\lights"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\lights\dot.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Profiling\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Release\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Release\lights"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\lights\light.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Profiling\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Release\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Release\lights"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\lights\light.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Profiling\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Release\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Release\lights"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\lights\square.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Profiling\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Release\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Release\lights"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\lights\square.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Profiling\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Release\lights"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\lights"
# PROP Intermediate_Dir "Release\lights"

!ENDIF 

# End Source File
# End Group
# Begin Group "boundings"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\boundings\aabb.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Profiling\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Release\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Release\boundings"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\boundings\aabb.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Profiling\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Release\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Release\boundings"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\boundings\bounding.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Profiling\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Release\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Release\boundings"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\boundings\bounding.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Profiling\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Release\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Release\boundings"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\boundings\sphere.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Profiling\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Release\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Release\boundings"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\boundings\sphere.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Profiling\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Release\boundings"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\boundings"
# PROP Intermediate_Dir "Release\boundings"

!ENDIF 

# End Source File
# End Group
# Begin Group "normals"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\normals\bumps.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Profiling\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Release\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Release\normals"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\normals\bumps.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Profiling\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Release\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Release\normals"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\normals\normal.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Profiling\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Release\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Release\normals"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\normals\normal.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Profiling\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Release\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Release\normals"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\normals\turbulence.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Profiling\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Release\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Release\normals"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\normals\turbulence.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Profiling\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Release\normals"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\normals"
# PROP Intermediate_Dir "Release\normals"

!ENDIF 

# End Source File
# End Group
# Begin Group "primitives"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\blob.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\blob.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\box.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\box.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\capsule.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\capsule.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\cone.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\cone.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\csg.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\csg.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\cylinder.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\cylinder.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\heart.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\heart.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\julia.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\julia.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\metaobjects.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\metaobjects.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\plane.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\plane.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\primitive.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\primitive.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\quadric.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\quadric.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\sor.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\sor.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\sphere.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\sphere.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\superellipsoid.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\superellipsoid.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\torus.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\torus.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\transformed.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\primitives\transformed.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Profiling\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\primitives"
# PROP Intermediate_Dir "Release\primitives"

!ENDIF 

# End Source File
# End Group
# Begin Group "ambients"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\ambients\ambient.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Profiling\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Release\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Release\ambients"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\ambients\ambient.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Profiling\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Release\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Release\ambients"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\ambients\hemispherical.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Profiling\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Release\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Release\ambients"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\ambients\hemispherical.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Profiling\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Release\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Release\ambients"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\ambients\solid.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Profiling\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Release\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Release\ambients"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\ambients\solid.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Profiling\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Release\ambients"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\ambients"
# PROP Intermediate_Dir "Release\ambients"

!ENDIF 

# End Source File
# End Group
# Begin Group "pigments"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\checker.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\checker.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\cubemap.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\cubemap.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\cylindrical.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\cylindrical.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\granite.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\granite.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\mandel.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\mandel.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\marble.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\marble.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\noisy.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\noisy.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\pigment.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\pigment.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\planar.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\planar.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\skymap.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\skymap.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\solid.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\solid.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\spherical.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\spherical.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\torusmap.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\torusmap.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\wood.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\pigments\wood.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Profiling\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\pigments"
# PROP Intermediate_Dir "Release\pigments"

!ENDIF 

# End Source File
# End Group
# Begin Group "atmospherics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\atmospherics\atmospheric.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Profiling\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\atmospherics\atmospheric.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Profiling\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\atmospherics\fog.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Profiling\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\atmospherics\fog.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Profiling\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\atmospherics\ground.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Profiling\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\atmospherics\ground.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Profiling\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\atmospherics\volumetric.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Profiling\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\atmospherics\volumetric.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Profiling\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\atmospherics"
# PROP Intermediate_Dir "Release\atmospherics"

!ENDIF 

# End Source File
# End Group
# Begin Group "text"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\text\basics.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\text"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\text"
# PROP Intermediate_Dir "Profiling\text"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\text"
# PROP Intermediate_Dir "Release\text"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\text"
# PROP Intermediate_Dir "Release\text"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\text\basics.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\text"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\text"
# PROP Intermediate_Dir "Profiling\text"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\text"
# PROP Intermediate_Dir "Release\text"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\text"
# PROP Intermediate_Dir "Release\text"

!ENDIF 

# End Source File
# End Group
# Begin Group "poly3d"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\poly3d\camerasimple.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Profiling\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Release\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Release\poly3d"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\poly3d\camerasimple.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Profiling\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Release\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Release\poly3d"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\poly3d\rendersimple.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Profiling\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Release\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Release\poly3d"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\poly3d\rendersimple.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Profiling\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Release\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Release\poly3d"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\poly3d\wire.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Profiling\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Release\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Release\poly3d"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\poly3d\wire.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Profiling\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Release\poly3d"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\poly3d"
# PROP Intermediate_Dir "Release\poly3d"

!ENDIF 

# End Source File
# End Group
# Begin Group "fonts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\fonts\font.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\fonts"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\fonts"
# PROP Intermediate_Dir "Profiling\fonts"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\fonts"
# PROP Intermediate_Dir "Release\fonts"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\fonts"
# PROP Intermediate_Dir "Release\fonts"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\fonts\font.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\fonts"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\fonts"
# PROP Intermediate_Dir "Profiling\fonts"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\fonts"
# PROP Intermediate_Dir "Release\fonts"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\fonts"
# PROP Intermediate_Dir "Release\fonts"

!ENDIF 

# End Source File
# End Group
# Begin Group "types"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\types\basics.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\types"
# PROP Intermediate_Dir "Profiling\types"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\types"
# PROP Intermediate_Dir "Release\types"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\types"
# PROP Intermediate_Dir "Release\types"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\types\bitvector.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\types"
# PROP Intermediate_Dir "Profiling\types"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\types"
# PROP Intermediate_Dir "Release\types"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\types"
# PROP Intermediate_Dir "Release\types"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\types\bitvector.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\types"
# PROP Intermediate_Dir "Profiling\types"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\types"
# PROP Intermediate_Dir "Release\types"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\types"
# PROP Intermediate_Dir "Release\types"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\types\integer.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP BASE Intermediate_Dir "Release\types"
# PROP Intermediate_Dir "Profiling\types"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP BASE Intermediate_Dir "Release\types"
# PROP Intermediate_Dir "Release\types"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP BASE Intermediate_Dir "Release\types"
# PROP Intermediate_Dir "Release\types"

!ENDIF 

# End Source File
# End Group
# Begin Group "partitions"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\partitions\bboxtree.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\partitions"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\partitions\bboxtree.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\partitions"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\partitions\bcylinder.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\partitions"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\partitions\bcylinder.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\partitions"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\partitions\bspheretree.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\partitions"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\partitions\bspheretree.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\partitions"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\partitions\kdtree.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\partitions"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\partitions\kdtree.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\partitions"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\partitions\uniformgrid.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\partitions"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\partitions\uniformgrid.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\partitions"

!ENDIF 

# End Source File
# End Group
# Begin Group "animators"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\animators\add.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\add.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\animator.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\animator.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\bezier.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\bezier.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\bounce.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\bounce.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\circle.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\circle.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\linearf.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\linearf.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\lissajou.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\lissajou.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\mover.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\animators\mover.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "profiling\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\animators"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\animators"

!ENDIF 

# End Source File
# End Group
# Begin Group "parser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\parser\context.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\context.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\execute.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\functions.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\functions.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\hashing.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\hashing.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\parser.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\parser.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\parser_skel.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\parser_skel.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\qualified.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\qualified.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\scanner.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\scanner.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\scene_parser.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\scene_parser.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\struct_skel.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\tag.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\tag.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\value.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\parser\value.h

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Intermediate_Dir "Debug\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Intermediate_Dir "Debug\profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release\parser"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release\parser"

!ENDIF 

# End Source File
# End Group
# Begin Group "cameras"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\sources\newray\src\cameras\camera.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\cameras"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\cameras\camera.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\cameras"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\cameras\classic.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\cameras"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\cameras\classic.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\cameras"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\cameras\perspective.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\cameras"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\cameras\perspective.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "release\cameras"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=c:\sources\newray\src\all_ambients.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\all_atmospherics.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\all_boundings.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\all_finishes.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\all_interiors.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\all_lights.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\all_movers.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\all_pigments.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\all_primitives.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\all_textures.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\blend.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\color.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\color.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\colormap.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\colormap.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\error.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\error.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\finish.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\finish.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\globals.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\globals.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\handle.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\handle.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\interior.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\interior.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\intersection.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\intersection.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\ior.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\jitter.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\jitter.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\lighting.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\lighting.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\main_common.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\main_common.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\main_dx.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "directX"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\main_ptc.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "openPTC"
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\memory.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\memory.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\object.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\object.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\random.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\ray.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\ray.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\statistics.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\statistics.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\texture.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\texture.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\trace.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\world.cpp

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=c:\sources\newray\src\world.h

!IF  "$(CFG)" == "newray - Win32 Debug"

!ELSEIF  "$(CFG)" == "newray - Win32 Profiling"

!ELSEIF  "$(CFG)" == "newray - Win32 directX"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "newray - Win32 openPTC"

# PROP Intermediate_Dir "Release"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
