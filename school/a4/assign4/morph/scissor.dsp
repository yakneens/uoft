# Microsoft Developer Studio Project File - Name="scissor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=scissor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "scissor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "scissor.mak" CFG="scissor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "scissor - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "scissor - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "scissor - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x1009 /d "NDEBUG"
# ADD RSC /l 0x1009 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "scissor - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /GX /ZI /Ot /Op /I "." /I ".." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /D "VC_EXTRA_LEAN" /D "WIN32_EXTRA_LEAN" /FR /YX /FD /o /win32 /c
# ADD BASE RSC /l 0x1009 /d "_DEBUG"
# ADD RSC /l 0x1009 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib fltk.lib fltkgl.lib base_md.lib newmat.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcd" /nodefaultlib:"libcmtd" /nodefaultlib:"libcimtd.lib" /nodefaultlib:"msvcrtd.lib" /libpath:"..\..\Software Distribution\lib"
# SUBTRACT LINK32 /pdb:none /incremental:no

!ENDIF 

# Begin Target

# Name "scissor - Win32 Release"
# Name "scissor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=".\c-code\boundary.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-code\bspline-draw.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-code\bspline.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-code\circle.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-code\color.cpp"
# End Source File
# Begin Source File

SOURCE=.\glutils.cpp
# End Source File
# Begin Source File

SOURCE=".\c-code\imageprimitive.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-code\imdraw-draw.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-code\imdraw-handle.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-code\imdraw-init.cpp"
# End Source File
# Begin Source File

SOURCE=".\imdraw-utils.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-code\line.cpp"
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=".\c-code\point.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-code\polygon.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-code\pqelement.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-code\readDrawing.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-code\rectangle.cpp"
# End Source File
# Begin Source File

SOURCE=.\ScissorUI.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\Texture.cpp
# End Source File
# Begin Source File

SOURCE=".\c-code\wavelets.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\c-code\boundary.h"
# End Source File
# Begin Source File

SOURCE=".\c-code\bspline.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-bspline.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-circle.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-clearall.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-color.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-convert-to-spline.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-level.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-line.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-load-boundary.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-load-seed-points.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-load-spline.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-open-drawing.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-open-image.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-point.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-poly.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-quit.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-rect.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-save-boundary.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-save-drawing.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-save-gradient.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-save-link-costs.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-save-matte.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-save-seed-points.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-save-zero-crossings.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-scissor.h"
# End Source File
# Begin Source File

SOURCE=".\fluid-hooks\callback-undo.h"
# End Source File
# Begin Source File

SOURCE=".\c-code\circle.h"
# End Source File
# Begin Source File

SOURCE=".\c-code\color.h"
# End Source File
# Begin Source File

SOURCE=.\glutils.h
# End Source File
# Begin Source File

SOURCE=".\c-code\imageprimitive.h"
# End Source File
# Begin Source File

SOURCE=".\c-code\imdraw.h"
# End Source File
# Begin Source File

SOURCE=".\c-code\init-scissor.h"
# End Source File
# Begin Source File

SOURCE=".\c-code\line.h"
# End Source File
# Begin Source File

SOURCE=".\c-code\point.h"
# End Source File
# Begin Source File

SOURCE=".\c-code\polygon.h"
# End Source File
# Begin Source File

SOURCE=".\c-code\pqelement.h"
# End Source File
# Begin Source File

SOURCE=".\c-code\rectangle.h"
# End Source File
# Begin Source File

SOURCE=.\ScissorUI.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\Texture.h
# End Source File
# Begin Source File

SOURCE=".\c-code\wavelets.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ScissorUI.fl

!IF  "$(CFG)" == "scissor - Win32 Release"

!ELSEIF  "$(CFG)" == "scissor - Win32 Debug"

# Begin Custom Build - Create ScissorUI.cpp and ScissorUI.h with fluid
InputPath=.\ScissorUI.fl

BuildCmds= \
	"..\..\..\Software Distribution\bin\fluid" -c ScissorUI.fl

"ScissorUI.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"ScissorUI.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
