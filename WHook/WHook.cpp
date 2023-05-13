// dll.cpp : Defines the entry point for the DLL application.
//
#include "windows.h"
#include "WHook.h"
#include <tchar.h>
#include "IPC.h"
//

struct tcom_vars {
    UINT IDM_MYMENU;
	UINT IDM_MYMENUALL;
	UINT WM_SETMENU;	
	UINT WM_REMOVEMENU;
	BOOL isAddHideIt;
	BOOL isAddHideAll;
	HHOOK prevHookGetMsg;
	HHOOK prevHookCallWnd;
	HHOOK prevHookCBT;
	HHOOK prevHookShell;
	HHOOK prevHookMouse;
	HWND hWndMain;
} com_vars = {0, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL};


// Global variables
HINSTANCE	hDllInst;				// Global instance handle for	DLL
static CIPC g_obIPC;				// Shared resources used to transfer data from PwdSpy to the hook code

LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);


BOOL CALLBACK DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	hDllInst = (HINSTANCE) hModule;
	return 1;
}

int CALLBACK WEP (int bSystemExit)
{
	return(1);
}


HINSTANCE InitHooksDll(INITPARMS initParms)
{
    com_vars.hWndMain = initParms.hMainWindow;
    com_vars.WM_SETMENU = initParms.wmSetMenu;
    com_vars.WM_REMOVEMENU = initParms.wmRemoveMenu;
	com_vars.isAddHideIt = initParms.isAddHideIt;
	com_vars.isAddHideAll = initParms.isAddHideAll;
    com_vars.IDM_MYMENU = initParms.IDM_MYMENU;
	com_vars.IDM_MYMENUALL = initParms.IDM_MYMENUALL;
	com_vars.prevHookGetMsg = NULL;
	com_vars.prevHookCallWnd = NULL;
	com_vars.prevHookCBT = NULL;
	com_vars.prevHookShell = NULL;
	com_vars.prevHookMouse = NULL;
    return hDllInst;
}

int InstallHooks(const BOOL action)
{
	if(action)
	{
		if(g_obIPC.Lock())
			g_obIPC.CreateIPCMMF();
		else
			return 10;
		com_vars.prevHookGetMsg = SetWindowsHookEx(WH_GETMESSAGE , GetMsgProc, hDllInst, 0); //WH_CALLWNDPROC
		if(com_vars.prevHookGetMsg == NULL)
		{
			g_obIPC.Unlock();
			return 1;
		}
		
		com_vars.prevHookCallWnd = SetWindowsHookEx(WH_CALLWNDPROC , CallWndProc, hDllInst, 0); //WH_CALLWNDPROC
		if(com_vars.prevHookCallWnd == NULL)
		{
			g_obIPC.Unlock();
			return 2;
		}
		
		com_vars.prevHookCBT = SetWindowsHookEx(WH_CBT , CBTProc, hDllInst, 0); //WH_CALLWNDPROC
		if(com_vars.prevHookCBT == NULL)
		{
			g_obIPC.Unlock();
			return 3;
		}
		/*
		com_vars.prevHookShell = SetWindowsHookEx(WH_SHELL , ShellProc, hDllInst, 0); //WH_CALLWNDPROC
		if(com_vars.prevHookShell == NULL)
		{
			g_obIPC.Unlock();
			InstallHooks(FALSE);
			return 4;
		}
		*/
		com_vars.prevHookMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, hDllInst, 0); //WH_CALLWNDPROC

		if(com_vars.prevHookMouse == NULL)
		{
			g_obIPC.Unlock();
			return 5;
		}
		// Into the shared resource, store the handle to the hook.
		g_obIPC.WriteIPCMMF((LPBYTE)&com_vars, sizeof(com_vars));
		g_obIPC.Unlock();
		return 0;
	} else
	{
		UnhookWindowsHookEx(com_vars.prevHookCallWnd);
		UnhookWindowsHookEx(com_vars.prevHookGetMsg);
		UnhookWindowsHookEx(com_vars.prevHookCBT);
		//UnhookWindowsHookEx(com_vars.prevHookShell);
		UnhookWindowsHookEx(com_vars.prevHookMouse);
		return 0;
	}
}

/*

LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if( (com_vars.prevHookCallWnd == NULL) || (com_vars.hWndMain == NULL) )
	{	// Read the data from the shared resources
		DWORD dwSize = sizeof(com_vars);
		g_obIPC.Lock();
		g_obIPC.OpenIPCMMF();
		g_obIPC.ReadIPCMMF((LPBYTE)&com_vars, dwSize);
		g_obIPC.Unlock();
	}
	//
	if(nCode == HSHELL_WINDOWCREATED)
	{

		//MessageBox(0,"create","create",0);
		PostMessage(com_vars.hWndMain, WM_APP+22, (WPARAM) GetCurrentThreadId(), (LPARAM) wParam);

		HMENU hMenu = NULL;
		hMenu = GetSystemMenu((HWND) wParam, FALSE);

		if (hMenu != NULL)				// если удалось все таки	
		{
			int isFound = 0;
			for(int i=0; i < GetMenuItemCount(hMenu); i++)
			{
				UINT m = GetMenuItemID(hMenu, i);
				if(m == com_vars.IDM_MYMENU)
				{
					isFound = 1;
					break;
				}
			}
			if(isFound==0)
			{
				AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
				AppendMenu(hMenu, MF_STRING | MF_ENABLED, com_vars.IDM_MYMENU , "Hide it window");
			}
			//	Hide All!
			if(com_vars.isAddHideAll)
			{
				int isFound = 0;
				for(int i=0; i < GetMenuItemCount(hMenu); i++)
				{
					UINT m = GetMenuItemID(hMenu, i);
					if(m == com_vars.IDM_MYMENUALL)
					{
						isFound = 1;
						break;
					}
				}
				if(isFound==0)
				{
					AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
					AppendMenu(hMenu, MF_STRING | MF_ENABLED, com_vars.IDM_MYMENUALL , "Hide all windows");
				}
			}
		}
	}
	//
	if(nCode == HSHELL_WINDOWDESTROYED)
	{
		PostMessage(com_vars.hWndMain, WM_APP+12, (WPARAM) GetCurrentThreadId(), (LPARAM) wParam);
	}
	return CallNextHookEx(com_vars.prevHookCBT, nCode, wParam, lParam);

}
*/

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if( (com_vars.prevHookMouse == NULL) || (com_vars.hWndMain == NULL) )
	{	// Read the data from the shared resources
		DWORD dwSize = sizeof(com_vars);
		if(g_obIPC.Lock())
		{
			g_obIPC.OpenIPCMMF();
			g_obIPC.ReadIPCMMF((LPBYTE)&com_vars, dwSize);
			g_obIPC.Unlock();
		}
	}
	//
	if(nCode >= 0)
	{
		if( wParam == WM_NCRBUTTONDOWN )
		{
			MOUSEHOOKSTRUCT *pMsg = (MOUSEHOOKSTRUCT*)lParam;
			if( (pMsg->wHitTestCode == HTMINBUTTON)||(pMsg->wHitTestCode == HTREDUCE) )
			{
				::SetForegroundWindow(pMsg->hwnd);
				PostMessage(com_vars.hWndMain, WM_APP+15, (WPARAM) GetCurrentThreadId(), (LPARAM) pMsg->hwnd);
			}
			//
			if( (pMsg->wHitTestCode == HTMAXBUTTON)||(pMsg->wHitTestCode == HTZOOM)||(pMsg->wHitTestCode == HTCLOSE) )
			{
				::SetForegroundWindow(pMsg->hwnd);
				PostMessage(com_vars.hWndMain, WM_APP+21, (WPARAM) GetCurrentThreadId(), (LPARAM) pMsg->hwnd);
			}
		}
		//
		if( wParam == WM_NCRBUTTONDBLCLK )
		{
			MOUSEHOOKSTRUCT *pMsg = (MOUSEHOOKSTRUCT*)lParam;
			::SetForegroundWindow(pMsg->hwnd);
			PostMessage(com_vars.hWndMain, WM_APP+18, (WPARAM) GetCurrentThreadId(), (LPARAM) pMsg->hwnd);
		}
	}
	//
	return CallNextHookEx(com_vars.prevHookMouse, nCode, wParam, lParam);
}

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if( (com_vars.prevHookCallWnd == NULL) || (com_vars.hWndMain == NULL) )
	{	// Read the data from the shared resources
		DWORD dwSize = sizeof(com_vars);
		if(g_obIPC.Lock())
		{
			g_obIPC.OpenIPCMMF();
			g_obIPC.ReadIPCMMF((LPBYTE)&com_vars, dwSize);
			g_obIPC.Unlock();
		}
	}
	//
	if(nCode == HCBT_CREATEWND)
	{

		PostMessage(com_vars.hWndMain, WM_APP+8, (WPARAM) GetCurrentThreadId(), (LPARAM) wParam);

		HMENU hMenu = NULL;
		hMenu = GetSystemMenu((HWND) wParam, FALSE);

		if (hMenu != NULL)				// если удалось все таки
		{
			if(com_vars.isAddHideIt)
			{
				int isFound = 0;
				for(int i=0; i < GetMenuItemCount(hMenu); i++)
				{
					UINT m = GetMenuItemID(hMenu, i);
					if(m == com_vars.IDM_MYMENU)
					{
						isFound = 1;
						break;
					}
				}
				if(isFound==0)
				{
					AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
					AppendMenu(hMenu, MF_STRING | MF_ENABLED, com_vars.IDM_MYMENU , "Hide it window");
				}
			}
			//	Hide All!
			if(com_vars.isAddHideAll)
			{
				int isFound = 0;
				for(int i=0; i < GetMenuItemCount(hMenu); i++)
				{
					UINT m = GetMenuItemID(hMenu, i);
					if(m == com_vars.IDM_MYMENUALL)
					{
						isFound = 1;
						break;
					}
				}
				if(isFound==0)
				{
					AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
					AppendMenu(hMenu, MF_STRING | MF_ENABLED, com_vars.IDM_MYMENUALL , "Hide all windows");
				}
			}
		}
	}

	if(nCode == HCBT_MINMAX)
	{
		if( LOWORD(lParam) == SW_MINIMIZE )
		{
			PostMessage(com_vars.hWndMain, WM_APP+14, (WPARAM) GetCurrentThreadId(), (LPARAM) wParam);
		}

	}
	//
	if(nCode == HCBT_DESTROYWND)
	{
		PostMessage(com_vars.hWndMain, WM_APP+12, (WPARAM) GetCurrentThreadId(), (LPARAM) wParam);
	}
	//
	return CallNextHookEx(com_vars.prevHookCBT, nCode, wParam, lParam);

}

LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if( (com_vars.prevHookCallWnd == NULL) || (com_vars.hWndMain == NULL) )
	{	// Read the data from the shared resources
		DWORD dwSize = sizeof(com_vars);
		if(g_obIPC.Lock())
		{
			g_obIPC.OpenIPCMMF();
			g_obIPC.ReadIPCMMF((LPBYTE)&com_vars, dwSize);
			g_obIPC.Unlock();
		}
	}

	if(nCode >= 0)
	{
		CWPSTRUCT *pMsg = (CWPSTRUCT*)lParam;
		UINT ms = pMsg->message;

		if( ms == com_vars.WM_SETMENU )
		{
			pMsg->message = NULL;
			HMENU hMenu = NULL;
			hMenu = GetSystemMenu(pMsg->hwnd, FALSE);
			if (hMenu != NULL)				// если удалось все таки	
			{
				if(com_vars.isAddHideIt)
				{
					int isFound = 0;
					for(int i=0; i < GetMenuItemCount(hMenu); i++)
					{
						UINT m = GetMenuItemID(hMenu, i);
						if(m == com_vars.IDM_MYMENU)
						{
							isFound = 1;
							break;
						}
					}
					if(isFound==0)
					{
						AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
						AppendMenu(hMenu, MF_STRING | MF_ENABLED, com_vars.IDM_MYMENU , "Hide it window");
					} else
					{
						ModifyMenu(hMenu, com_vars.IDM_MYMENU, MF_BYCOMMAND | MF_ENABLED |
							MF_STRING, com_vars.IDM_MYMENU, "Hide It!"); 
					}
				}
				//	Hide All!
				if(com_vars.isAddHideAll)
				{
					int isFound = 0;
					for(int i=0; i < GetMenuItemCount(hMenu); i++)
					{
						UINT m = GetMenuItemID(hMenu, i);
						if(m == com_vars.IDM_MYMENUALL)
						{
							isFound = 1;
							break;
						}
					}
					if(isFound==0)
					{
						AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
						AppendMenu(hMenu, MF_STRING | MF_ENABLED, com_vars.IDM_MYMENUALL , "Hide all windows");
					} else
					{
						ModifyMenu(hMenu, com_vars.IDM_MYMENUALL, MF_BYCOMMAND | MF_ENABLED |
							MF_STRING, com_vars.IDM_MYMENUALL, "Hide all windows"); 
					}
				}
			}
		}
		//
		if( ms == com_vars.WM_REMOVEMENU )
		{
			PostMessage(com_vars.hWndMain, WM_APP+11, (WPARAM) GetCurrentThreadId(), (LPARAM)pMsg->hwnd);
			pMsg->message = NULL;
			HMENU hMenu = NULL;
			hMenu = GetSystemMenu(pMsg->hwnd, FALSE);
			if (hMenu != NULL)				// если удалось все таки	
			{
				if(com_vars.isAddHideIt)
				{
					for(int i=0; i < GetMenuItemCount(hMenu); i++)
					{
						UINT m = GetMenuItemID(hMenu, i);
						if(m == com_vars.IDM_MYMENU)
						{
							RemoveMenu(hMenu, i , MF_BYPOSITION);
							RemoveMenu(hMenu, i - 1, MF_BYPOSITION);
							break;
						}
					}
				}
				//	Hide All!
				if(com_vars.isAddHideAll)
				{
					for(int i=0; i < GetMenuItemCount(hMenu); i++)
					{
						UINT m = GetMenuItemID(hMenu, i);
						if(m == com_vars.IDM_MYMENUALL)
						{
							RemoveMenu(hMenu, i , MF_BYPOSITION);
							RemoveMenu(hMenu, i - 1, MF_BYPOSITION);
							break;
						}
					}
				}
			}
		}
	}
	return CallNextHookEx(com_vars.prevHookCallWnd, nCode, wParam, lParam);

}

LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if( (com_vars.prevHookGetMsg == NULL) || (com_vars.hWndMain == NULL) )
	{	// Read the data from the shared resources
		DWORD dwSize = sizeof(com_vars);
		if(g_obIPC.Lock())
		{
			g_obIPC.OpenIPCMMF();
			g_obIPC.ReadIPCMMF((LPBYTE)&com_vars, dwSize);
			g_obIPC.Unlock();
		}
	}

	if(nCode >= 0)
	{
		MSG *pMsg = (MSG*)lParam;
		UINT ms = pMsg->message;

		if( ms == com_vars.WM_SETMENU )
		{
			pMsg->message = NULL;
			HMENU hMenu = NULL;
			hMenu = GetSystemMenu(pMsg->hwnd, FALSE);
			if (hMenu != NULL)				// если удалось все таки	
			{
				if(com_vars.isAddHideIt)
				{
					int isFound = 0;
					for(int i=0; i < GetMenuItemCount(hMenu); i++)
					{
						UINT m = GetMenuItemID(hMenu, i);
						if(m == com_vars.IDM_MYMENU)
						{
							isFound = 1;
							break;
						}
					}
					if(isFound==0)
					{
						AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
						AppendMenu(hMenu, MF_STRING | MF_ENABLED, com_vars.IDM_MYMENU , "Hide it window");
					} else
					{
						ModifyMenu(hMenu, com_vars.IDM_MYMENU, MF_BYCOMMAND | MF_ENABLED |
							MF_STRING, com_vars.IDM_MYMENU, "Hide it window"); 
					}
				}
				//	Hide All!
				if(com_vars.isAddHideAll)
				{
					int isFound = 0;
					for(int i=0; i < GetMenuItemCount(hMenu); i++)
					{
						UINT m = GetMenuItemID(hMenu, i);
						if(m == com_vars.IDM_MYMENUALL)
						{
							isFound = 1;
							break;
						}
					}
					if(isFound==0)
					{
						AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
						AppendMenu(hMenu, MF_STRING | MF_ENABLED, com_vars.IDM_MYMENUALL , "Hide all windows");
					} else
					{
						ModifyMenu(hMenu, com_vars.IDM_MYMENUALL, MF_BYCOMMAND | MF_ENABLED |
							MF_STRING, com_vars.IDM_MYMENUALL, "Hide all windows"); 
					}
				}
			}
		}
		//
		if( ms == com_vars.WM_REMOVEMENU )
		{
			PostMessage(com_vars.hWndMain, WM_APP+11, (WPARAM) GetCurrentThreadId(), (LPARAM)pMsg->hwnd);
			pMsg->message = NULL;
			HMENU hMenu = NULL;
			hMenu = GetSystemMenu(pMsg->hwnd, FALSE);
			if (hMenu != NULL)				// если удалось все таки	
			{
				if(com_vars.isAddHideIt)
				{
					for(int i=0; i < GetMenuItemCount(hMenu); i++)
					{
						UINT m = GetMenuItemID(hMenu, i);
						if(m == com_vars.IDM_MYMENU)
						{
							RemoveMenu(hMenu, i , MF_BYPOSITION);
							RemoveMenu(hMenu, i - 1, MF_BYPOSITION);
							break;
						}
					}
				}
				//	Hide All!
				if(com_vars.isAddHideAll)
				{
					for(int i=0; i < GetMenuItemCount(hMenu); i++)
					{
						UINT m = GetMenuItemID(hMenu, i);
						if(m == com_vars.IDM_MYMENUALL)
						{
							RemoveMenu(hMenu, i , MF_BYPOSITION);
							RemoveMenu(hMenu, i - 1, MF_BYPOSITION);
							break;
						}
					}
				}
			}
		}
		//
		if( ms == WM_SYSCOMMAND )
		{
			if( (pMsg->wParam & 0xFFF0) == com_vars.IDM_MYMENU)
			{
				pMsg->message = NULL;
					//HeapCompact(GetProcessHeap(),HEAP_NO_SERIALIZE);
				PostMessage(com_vars.hWndMain, WM_APP+10, (WPARAM) GetCurrentThreadId(), (LPARAM)pMsg->hwnd);
			} 
			if( (pMsg->wParam & 0xFFF0) == com_vars.IDM_MYMENUALL)
			{
				PostMessage(com_vars.hWndMain, WM_APP+9, (WPARAM) GetCurrentThreadId(), (LPARAM)pMsg->hwnd);
				pMsg->message = NULL;
			}
		}
	
	}
	// Always call the next hook in the chain
	return CallNextHookEx(com_vars.prevHookGetMsg, nCode, wParam, lParam);
}

