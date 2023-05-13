#if !defined(AFX_APPINFO_H__61406653_13BE_11D6_AF44_00D0B75DC76B__INCLUDED_)
#define AFX_APPINFO_H__61406653_13BE_11D6_AF44_00D0B75DC76B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AppInfo.h : header file
//
#include "SystemTray.h"
/////////////////////////////////////////////////////////////////////////////
// CAppInfo view

class CWndDescr : public CObject
{
// Attributes
public:
    DECLARE_DYNAMIC(CWndDescr)
	CWndDescr();
	~CWndDescr();
public:
	DWORD			m_wndId;
	HWND			m_hWnd;
	HWND			m_hWndParent;
	HWND			m_foregroundWindow;
	CString			m_windowText;
	CString			m_programName;
	HICON			m_bigIcon;
	HICON			m_smallIcon;
	CSystemTray		m_SystemTray;
	DWORD			m_processId;
	DWORD			m_threadId;
	DWORD			m_priorityClass;
	BOOL			m_isPriorityChanged;
	CTime			m_createTime;
	RECT			m_wndRect;
	int				m_typeHidden;
	int				m_isMaster;
};

typedef  CWndDescr*  LPWNDDESCR;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPINFO_H__61406653_13BE_11D6_AF44_00D0B75DC76B__INCLUDED_)
