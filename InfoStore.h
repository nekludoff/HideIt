//
#ifndef __INFOSTORE_H__
#define __INFOSTORE_H__
#endif
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoStore.h : header file
//
typedef struct tagInfoStore
{
	DWORD dwInfoId;
	HWND hWnd;
	DWORD dwProcessId;
	DWORD dwThreadId;
	DWORD dwPriority;
	DWORD dwState;
} INFOSTORE, *LPINFOSTORE;


/////////////////////////////////////////////////////////////////////////////
// CInfoStore

class CInfoStore : public CFile
{
// Construction
public:
	CInfoStore();
	int Create(LPCTSTR lpszFileName);
	int Open(LPCTSTR lpszFileName);
	~CInfoStore();
	BOOL ReadInfo(DWORD dwInfoId, LPINFOSTORE infoStore);
	BOOL WriteInfo(INFOSTORE infoStore);
private:
	LONG GetFreePosition();
	LPCTSTR m_lpszFileName;
	BOOL m_isOpen;
};
