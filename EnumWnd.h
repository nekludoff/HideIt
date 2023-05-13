#include "psapi.h" // for the process handling, needs psapi.dll which is not distributed with windows.
					// note: psapi is a windows NT/2000 specific DLL (modified to work with win9x as well)
#include <tlhelp32.h> // for windows 98
#include <vdmdbg.h> // for ntvdm 16-bit

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWndInfo : public CObject
{
public:
	HWND hWnd;
	DWORD processId;
	DWORD threadId;
	CString wText;
	BOOL	isVisible;
};

typedef  CWndInfo*  LPWNDINFO;

class CProcessInfo : public CObject
{
public:
	DWORD	pid;
};

typedef  CProcessInfo*  LPPROCESSINFO;

typedef struct tagENUMPARMS {
	CObArray* p_wndInfoArray;
	CString skipText;
	DWORD parms;
	DWORD processId;
	DWORD threadId;
} ENUMPARMS, *LPENUMPARMS;

class CEnumWnd : public CObject
{
// Construction
public:
	CEnumWnd();
	~CEnumWnd();
	BOOL EnumProcesses();
	BOOL EnumWnd(DWORD parm = enumSKIPPROGMAN_EMPTY_INV);
	BOOL EnumWnd(DWORD parm, DWORD processId);
	BOOL EnumChildWnd(HWND parentWnd, DWORD parm = enumSKIPPROGMAN_EMPTY_INV);
	enum {
		enumSKIPEMPTYCAPTION = 0x0001,
		enumSKIPPROGMAN = 0x0002,
		enumSKIPPROGMAN_EMPTY = 0x0003,
		enumSKIPINVISIBLE = 0x0004,
		enumSKIPPROGMAN_EMPTY_INV = 0x0007,
		enumFINDPARENT = 0x0008,
		enumFINDROOT = 0x0016
	};
public:
	CObArray m_wndInfoArray;
	CObArray m_processesInfoArray;
};
