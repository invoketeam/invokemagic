# Microsoft Developer Studio Project File - Name="invokeProto2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=invokeProto2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "invokeProto2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "invokeProto2.mak" CFG="invokeProto2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "invokeProto2 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "invokeProto2 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "invokeProto2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "invokeProto2 - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "invokeProto2 - Win32 Release"
# Name "invokeProto2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "gamex"

# PROP Default_Filter ""
# Begin Group "pugi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gamex\pugixml\pugixml.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\pugixml\pugixml.hpp
# End Source File
# End Group
# Begin Group "stb"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gamex\stb\miniz.c
# End Source File
# Begin Source File

SOURCE=.\gamex\stb\miniz.h
# End Source File
# Begin Source File

SOURCE=.\gamex\stb\stb_image.c
# End Source File
# Begin Source File

SOURCE=.\gamex\stb\stb_image.h
# End Source File
# Begin Source File

SOURCE=.\gamex\stb\stb_vorbis.c
# End Source File
# Begin Source File

SOURCE=.\gamex\stb\stb_vorbis.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\gamex\gamex.h
# End Source File
# Begin Source File

SOURCE=.\gamex\oglExt.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\oglExt.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xActor.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xActor.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xAsset.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xAsset.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xBigPart.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xBigPart.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xCam.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xCam.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xColMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xColMesh.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xCursor.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xDebug.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xDrawMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xDrawMesh.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xFlatRender.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xFlatRender.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xFont.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xFont.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xFrustum.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xFrustum.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xGame.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xGame.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xGameExt.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xGameExt.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xGLCommon.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xGuiGame.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xGuiGame.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xHeightMap.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xHeightMap.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xImage.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xImage.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xKey.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xMath.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xMdx3.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xMdx3.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xMultiGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xMultiGrid.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xMultiMdx3.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xMultiMdx3.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xPartSys.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xPartSys.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xPath.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xPathFind.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xPathFind.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xRand.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xRand.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xRender.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xRender.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xShadowTex.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xShadowTex.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xSkel.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xSkel.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xSound.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xSound.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xSprite.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xStore.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xStore.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xTexture.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xTileMap.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xTileMap.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xTmx.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xTmx.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xWin.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xWin.h
# End Source File
# Begin Source File

SOURCE=.\gamex\xWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\gamex\xWorld.h
# End Source File
# End Group
# Begin Group "invoke"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\invoke\invokeGame.cpp
# End Source File
# Begin Source File

SOURCE=.\invoke\invokeGame.h
# End Source File
# Begin Source File

SOURCE=.\invoke\invokeMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\invoke\invokeMenu.h
# End Source File
# Begin Source File

SOURCE=.\invoke\xBuildTest.cpp
# End Source File
# Begin Source File

SOURCE=.\invoke\xBuildTest.h
# End Source File
# Begin Source File

SOURCE=.\invoke\xGroundDecal.cpp
# End Source File
# Begin Source File

SOURCE=.\invoke\xGroundDecal.h
# End Source File
# Begin Source File

SOURCE=.\invoke\xHand.cpp
# End Source File
# Begin Source File

SOURCE=.\invoke\xHand.h
# End Source File
# Begin Source File

SOURCE=.\invoke\xInvActor.cpp
# End Source File
# Begin Source File

SOURCE=.\invoke\xInvActor.h
# End Source File
# Begin Source File

SOURCE=.\invoke\xInvokeCommon.h
# End Source File
# Begin Source File

SOURCE=.\invoke\xMiniMap.cpp
# End Source File
# Begin Source File

SOURCE=.\invoke\xMiniMap.h
# End Source File
# Begin Source File

SOURCE=.\invoke\xPartTest.cpp
# End Source File
# Begin Source File

SOURCE=.\invoke\xPartTest.h
# End Source File
# Begin Source File

SOURCE=.\invoke\xSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\invoke\xSelect.h
# End Source File
# Begin Source File

SOURCE=.\invoke\xTree.cpp
# End Source File
# Begin Source File

SOURCE=.\invoke\xTree.h
# End Source File
# Begin Source File

SOURCE=.\invoke\xUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\invoke\xUnit.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
