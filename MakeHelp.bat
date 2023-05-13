@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by HIDE.HPJ. >"hlp\Hide.hm"
echo. >>"hlp\Hide.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\Hide.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\Hide.hm"
echo. >>"hlp\Hide.hm"
echo // Prompts (IDP_*) >>"hlp\Hide.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\Hide.hm"
echo. >>"hlp\Hide.hm"
echo // Resources (IDR_*) >>"hlp\Hide.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\Hide.hm"
echo. >>"hlp\Hide.hm"
echo // Dialogs (IDD_*) >>"hlp\Hide.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\Hide.hm"
echo. >>"hlp\Hide.hm"
echo // Frame Controls (IDW_*) >>"hlp\Hide.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\Hide.hm"
REM -- Make help for Project HIDE


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\Hide.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\Hide.hlp" goto :Error
if not exist "hlp\Hide.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\Hide.hlp" Debug
if exist Debug\nul copy "hlp\Hide.cnt" Debug
if exist Release\nul copy "hlp\Hide.hlp" Release
if exist Release\nul copy "hlp\Hide.cnt" Release
echo.
goto :done

:Error
echo hlp\Hide.hpj(1) : error: Problem encountered creating help file

:done
echo.
