# Microsoft Developer Studio Generated NMAKE File, Based on WHook.dsp
!IF "$(CFG)" == ""
CFG=WHook - Win32 Debug
!MESSAGE No configuration specified. Defaulting to WHook - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "WHook - Win32 Release" && "$(CFG)" != "WHook - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WHook.mak" CFG="WHook - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WHook - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WHook - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WHook - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\WHook.dll"


CLEAN :
	-@erase "$(INTDIR)\WHook.obj"
	-@erase "$(INTDIR)\IPC.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\WHook.dll"
	-@erase "..\WHook.exp"
	-@erase "..\WHook.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WHook_EXPORTS" /Fp"$(INTDIR)\WHook.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WHook.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\WHook.pdb" /machine:I386 /def:".\WHook.def" /out:"../WHook.dll" /implib:"../WHook.lib" 
LINK32_OBJS= \
	"$(INTDIR)\WHook.obj" \
	"$(INTDIR)\IPC.obj"

"..\WHook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "WHook - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\WHook.dll"


CLEAN :
	-@erase "$(INTDIR)\WHook.obj"
	-@erase "$(INTDIR)\IPC.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\WHook.pdb"
	-@erase "..\WHook.dll"
	-@erase "..\WHook.exp"
	-@erase "..\WHook.ilk"
	-@erase "..\WHook.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "_USRDLL" /D "WHook_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_BETATEST" /Fp"$(INTDIR)\WHook.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\WHook.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\WHook.pdb" /debug /machine:I386 /def:".\WHook.def" /out:"../WHook.dll" /implib:"../WHook.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\WHook.obj" \
	"$(INTDIR)\IPC.obj"

"..\WHook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("WHook.dep")
!INCLUDE "WHook.dep"
!ELSE 
!MESSAGE Warning: cannot find "WHook.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "WHook - Win32 Release" || "$(CFG)" == "WHook - Win32 Debug"
SOURCE=.\WHook.cpp

"$(INTDIR)\WHook.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\IPC.cpp

"$(INTDIR)\IPC.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

