//
#ifndef __WndStore_H__
#define __WndStore_H__
#endif
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndStore.h : header file
//
class CWndInfoStore : public CObject
{
public:
   CWndInfoStore();
   ~CWndInfoStore();
   void Serialize(CArchive& ar);
public:
	CString	m_wndName;
	CString	m_progName;
	BOOL	m_isHideWhenMinimize;
	BOOL	m_isHideWhenCreated;
	BOOL	m_isHideByRMouse;
	BOOL	m_isHideByDblRMouse;
	int		m_hideMethod;
	BOOL	m_isShowOnActivateApp;
	int		m_hideTo;
	BOOL	m_isActivateNextWindow;
	long	m_wHash;
	BOOL	m_isPwdUseShowWindow;
	BOOL	m_isChangeProcessPriority;
	int		m_processPriority;
	BOOL	m_isDontChangeProcessPriorityPM;
	BOOL	m_isReduceProcessMemory;

	DECLARE_SERIAL(CWndInfoStore)
};

typedef  CWndInfoStore*  LPWNDINFOSTORE;

/////////////////////////////////////////////////////////////////////////////
// CWndStore

class CWndStore : public CFile
{
// Construction
public:
	CWndStore();
	~CWndStore();
	BOOL ReadInfo(LPCTSTR lpszFileName);
	BOOL WriteInfo(LPCTSTR lpszFileName);
private:
	LPCTSTR m_lpszFileName;
	BOOL m_isOpen;
};
