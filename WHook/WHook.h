#include "windows.h"

typedef struct tagInitParms{
	UINT IDM_MYMENU;
	UINT IDM_MYMENUALL;
    HWND hMainWindow;
	UINT wmSetMenu;
    UINT wmRemoveMenu;
	BOOL isAddHideIt;
	BOOL isAddHideAll;
} INITPARMS, *LPINITPARMS;

extern "C"
{

HINSTANCE InitHooksDll(INITPARMS initParms);
int InstallHooks(const BOOL action);

}