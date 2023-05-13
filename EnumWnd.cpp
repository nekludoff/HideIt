#include "stdafx.h"
#include "EnumWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam);
BOOL CALLBACK EnumChildWndProc(HWND hWnd, LPARAM lParam);

CEnumWnd::CEnumWnd()
{
	m_wndInfoArray.RemoveAll();
	m_processesInfoArray.RemoveAll();
}

CEnumWnd::~CEnumWnd()
{
	int i = 0;
	for (i=0;i < m_wndInfoArray.GetSize();i++)
	{
		LPWNDINFO wndInfo = (LPWNDINFO) m_wndInfoArray.ElementAt(i);
		delete wndInfo;
	}
	m_wndInfoArray.RemoveAll();
	//
	for (i=0;i < m_processesInfoArray.GetSize();i++)
	{
		LPPROCESSINFO processInfo = (LPPROCESSINFO) m_processesInfoArray.ElementAt(i);
		delete processInfo;
	}
	m_wndInfoArray.RemoveAll();
}

BOOL CEnumWnd::EnumWnd(DWORD parm, DWORD processId)
{
	BOOL result = FALSE;
	ENUMPARMS parms;
	
	m_wndInfoArray.RemoveAll();
	parms.parms = parm;
	parms.p_wndInfoArray = &m_wndInfoArray;
	parms.processId = processId;
	parms.threadId = 0;
	
	result = EnumWindows(EnumWndProc, (LPARAM) &parms);

	return (result);
}

BOOL CEnumWnd::EnumWnd(DWORD parm)
{
	BOOL result = FALSE;
	ENUMPARMS parms;
	
	m_wndInfoArray.RemoveAll();
	parms.parms = parm;
	parms.p_wndInfoArray = &m_wndInfoArray;
	parms.processId = 0;
	parms.threadId = 0;
	
	result = EnumWindows(EnumWndProc, (LPARAM) &parms);

	return (result);
}

BOOL CEnumWnd::EnumChildWnd(HWND parentWnd, DWORD parm)
{
	BOOL result = FALSE;
	ENUMPARMS parms;

	m_wndInfoArray.RemoveAll();
	parms.parms = parm;
	parms.p_wndInfoArray = &m_wndInfoArray;

	if( parm & CEnumWnd::enumFINDPARENT )
	{

	}

	result = EnumChildWindows(parentWnd, EnumChildWndProc, (LPARAM) &parms);

	return (result);
}

BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam)
{ 
	LPENUMPARMS parms = (LPENUMPARMS) lParam;
	CObArray* wndArray = parms->p_wndInfoArray;
	CWnd* wnd = CWnd::FromHandle(hWnd);
	BOOL isVisible;
	DWORD tid, pid;

	ASSERT(wnd);

	if(IsWindow(hWnd))
	{
		isVisible = wnd->IsWindowVisible();
		if( ( !isVisible ) && (parms->parms &  CEnumWnd::enumSKIPINVISIBLE) )
			return TRUE;
		//
		CString cs;
		wnd->GetWindowText(cs);
		if( (cs == "Program Manager") && (parms->parms &  CEnumWnd::enumSKIPPROGMAN) )
			return TRUE;
		if( cs.IsEmpty() && (parms->parms &  CEnumWnd::enumSKIPEMPTYCAPTION) )
			return TRUE;
		if(parms->processId != 0)
		{
			tid = GetWindowThreadProcessId(hWnd, &pid);
			if(pid != parms->processId)
				return TRUE;
		}
		//
		tid = GetWindowThreadProcessId(hWnd, &pid);
		LPWNDINFO wndInfo = new CWndInfo;
		wndInfo->hWnd = hWnd;
		wndInfo->processId = pid;
		wndInfo->threadId = tid;
		wndInfo->wText = cs;
		wndInfo->isVisible = isVisible;
		
		wndArray->Add(wndInfo);
	}
	return TRUE; 
} 

BOOL CALLBACK EnumChildWndProc(HWND hWnd, LPARAM lParam)
{ 
	LPENUMPARMS parms = (LPENUMPARMS) lParam;
	CObArray* wndArray = parms->p_wndInfoArray;
	CWnd* wnd = CWnd::FromHandle(hWnd);
	BOOL isVisible;

	if(IsWindow(hWnd))
	{
		isVisible = wnd->IsWindowVisible();
		CString cs;
		wnd->GetWindowText(cs);
		//
		LPWNDINFO wndInfo = new CWndInfo;
		wndInfo->hWnd = hWnd;
		wndInfo->wText = cs;
		wndInfo->isVisible = isVisible;
		
		wndArray->Add(wndInfo);
	}
	return TRUE; 
} 


int CEnumWnd::EnumProcesses()
{
	m_processesInfoArray.RemoveAll();
	
	OSVERSIONINFO osver;

	// get version
	osver.dwOSVersionInfoSize = sizeof(osver);
	if (!GetVersionEx(&osver)) return FALSE; // failed to obtain version information

	if (osver.dwPlatformId == VER_PLATFORM_WIN32_NT) // windows NT
	{
		// explicitly load the PSAPI dll so we don't get problems running under win9x
		HINSTANCE hLib = LoadLibraryA("PSAPI.DLL");
		if (!hLib) return 0; // could not load psapi.dll

		HINSTANCE hVdmLib = LoadLibraryA("VDMDBG.DLL"); // for shell windows and anything running under the VDM
		if (!hVdmLib)
		{
			// unload psapi
			FreeLibrary(hLib);
			return 0;
		}

		// function defs for PSAPI
		BOOL (WINAPI *lpfEnumProcesses)( DWORD *, DWORD cb, DWORD * );
		BOOL (WINAPI *lpfEnumProcessModules)( HANDLE, HMODULE *, DWORD, LPDWORD );
		DWORD (WINAPI *lpfGetModuleFileNameEx)( HANDLE, HMODULE, LPTSTR, DWORD );

		// function defs for NTVDM
		INT (WINAPI *lpfVDMEnumTaskWOWEx)( DWORD, TASKENUMPROCEX  fp, LPARAM );

		// get procedure handles
		lpfEnumProcesses = (BOOL(WINAPI *)(DWORD *,DWORD,DWORD*)) GetProcAddress( hLib, "EnumProcesses" ) ;
		lpfEnumProcessModules = (BOOL(WINAPI *)(HANDLE, HMODULE *, DWORD, LPDWORD)) GetProcAddress( hLib, "EnumProcessModules" ) ;
		lpfGetModuleFileNameEx =(DWORD (WINAPI *)(HANDLE, HMODULE, LPTSTR, DWORD )) GetProcAddress( hLib, "GetModuleFileNameExA" ) ;
		lpfVDMEnumTaskWOWEx =(INT(WINAPI *)( DWORD, TASKENUMPROCEX,	LPARAM))GetProcAddress( hVdmLib, "VDMEnumTaskWOWEx" );
		if( lpfEnumProcesses == NULL ||	lpfEnumProcessModules == NULL || lpfGetModuleFileNameEx == NULL || lpfVDMEnumTaskWOWEx == NULL)
		{
			// some of the functions did not load properly... return out
			FreeLibrary(hLib);
			FreeLibrary(hVdmLib);
			return 0;
		}

		// enumerate processes
		DWORD *pdwProcessIDs = new DWORD[1000]; // should be a good number of processes.  Change if you are using on a server
				// that might be running more than 250 processes.
				// change ti below in the EnumProcesses line as well. :)
		ASSERT(pdwProcessIDs);
		DWORD dwSize;

		if (!lpfEnumProcesses(pdwProcessIDs, 1000*sizeof(DWORD), &dwSize))
		{
			delete [] pdwProcessIDs;
			FreeLibrary(hLib);
			FreeLibrary(hVdmLib);
			return 0;
		}

		// get the exe names
		//HANDLE hProcess;
		//HMODULE hModule;
		//DWORD dwSize2;
		//char szFilename[_MAX_PATH];
		dwSize = dwSize / sizeof(DWORD);
		for (int i=0;i<(int)dwSize;i++)
		{
			LPPROCESSINFO processInfo = new CProcessInfo;
			processInfo->pid = pdwProcessIDs[i];
			m_processesInfoArray.Add(processInfo);
		}
		delete [] pdwProcessIDs;
	}
	else if (osver.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) // windows 9x
	{
		// load the kernel32 lib (explicitly)
		HINSTANCE hLib = LoadLibraryA("Kernel32.DLL");
		if (!hLib) return 0;

		// ToolHelp Function Pointers.
		HANDLE (WINAPI *lpfCreateToolhelp32Snapshot)(DWORD,DWORD) ;
		BOOL (WINAPI *lpfProcess32First)(HANDLE,LPPROCESSENTRY32) ;
		BOOL (WINAPI *lpfProcess32Next)(HANDLE,LPPROCESSENTRY32) ;

		// get procedure pointers
		lpfCreateToolhelp32Snapshot = (HANDLE(WINAPI *)(DWORD,DWORD)) GetProcAddress( hLib, "CreateToolhelp32Snapshot");
		lpfProcess32First = (BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))GetProcAddress( hLib, "Process32First");
		lpfProcess32Next = (BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))GetProcAddress( hLib, "Process32Next");
		if( lpfProcess32Next == NULL ||	lpfProcess32First == NULL || lpfCreateToolhelp32Snapshot == NULL)
		{
			FreeLibrary(hLib);
			return 0;
		}

		// Get a handle to a Toolhelp snapshot of the systems
		// processes.
		HANDLE hSnapShot = lpfCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if(hSnapShot == INVALID_HANDLE_VALUE)
		{
			FreeLibrary(hLib);
			return 0;
		}

		// get the first process entry
		PROCESSENTRY32 procentry;
		procentry.dwSize = sizeof(PROCESSENTRY32);
		BOOL bFlag = lpfProcess32First( hSnapShot, &procentry );

		while (bFlag)
		{
			// check filtering
			LPPROCESSINFO processInfo = new CProcessInfo;
			processInfo->pid = procentry.th32ProcessID;
			m_processesInfoArray.Add(processInfo);

			// get next process entry
			procentry.dwSize = sizeof(PROCESSENTRY32);
			bFlag = lpfProcess32Next( hSnapShot, &procentry );
		}
	}
	else return FALSE;

	return TRUE;
}
