// Hide.h : main header file for the HIDE application
//

#if !defined(AFX_HIDE_H__F484B4B8_1D82_11D6_8D1C_94DA39A0DB44__INCLUDED_)
#define AFX_HIDE_H__F484B4B8_1D82_11D6_8D1C_94DA39A0DB44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "SystemTray.h"
#include "OSInfo.h"
#include "InfoStore.h"
#include "WndStore.h"
#include "SplashWindow.h"	// Added by ClassView
#include "Afxmt.h"	// Added by ClassView
/////////////////////////////////////////////////////////////////////////////
// CHideApp:
// See Hide.cpp for the implementation of this class
//

class CHideApp : public CWinApp
{
public:
	CHideApp();
	~CHideApp();
	void ExitInstanceProcess(void);
	CWinThread* p_OptimizeAllThread;
	CWinThread* p_OptimizeHiddenThread;
	CWinThread* p_CheckWndInListThread;
	CWinThread* p_CheckAutoHideWndThread;
	CSplashWindow* m_pSplashWindow;
	void OptimizeMemoryUsage(void);
	COSInfo m_osi;
	UINT m_wmRemoveMenuMSG;
	UINT m_wmSetMenuMSG;
	DWORD GetMinWndId(void);
	DWORD m_minWndId;
	CObArray m_wndArray;
	CObArray m_wndInfoStoreArray;
	INT m_countHWND;
	HINSTANCE m_dllInstance;
	void ModifyAllWindows(BOOL sFlag);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHideApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	BOOL m_isDontChangeProcessPriorityPM;
	void GetPMThreadProcessId();
	DWORD m_PMThreadId;
	DWORD m_PMProcessId;
	BOOL m_isActivateNextWindow;
	CString m_hotKeyActionNames[10];
	int m_hotKeyArray[10][3];
	BOOL RestoreOnLog();
	CString m_infoStoreFileName;
	CString m_wndInfoStoreFileName;
	CInfoStore m_infoStore;
	CWndStore m_wndStore;
	CImageList m_wndInfoStoreImageList;
	DWORD	m_globalVisibleWndCount;
	DWORD	m_globalInVisibleWndCount;
	DWORD	m_globalVisibleInVisibleWndCount;
	int m_hideMethod;
	int m_processPriority;
	BOOL m_isChangeProcessPriority;
	int m_hideTo;
	void ReleaseWndId(DWORD wndId);
	UINT m_wmCommandMSG[150][2];
	UINT m_WM_TASKBARCREATED;
	BOOL m_isShowOnActivateApp;
	BOOL m_isHideWhenCreated;
	int m_typeShowAll;
	BOOL m_isHideByRMouse;
	BOOL m_isHideByDblRMouse;
	UINT m_IDM_MYMENUALL;
	UINT m_IDM_MYMENU;
	BOOL InstallWHook();
	BOOL m_isSoftExit;
	BOOL m_isTrial;
	int m_trialDaysLeft;
	int m_trialDaysTotal;
	CString m_userName;
	BOOL m_isAutoStart;
	BOOL m_isExtractIconFromFile;
	CCriticalSection m_CS;
	CCriticalSection m_CSList;
	BOOL m_isAddHideIt;
	BOOL m_isAddHideAll;
	BOOL m_isSplashWindow;
	BOOL m_isProcessInvisibleWindows;
	BOOL m_isHideWhenMinimize;
	BOOL m_isShowPopupWndMenu;
	BOOL	m_isSmallIcons;
	BOOL	m_isTrimCaption;
	CString	m_trimCaptionChars;
	BOOL	m_isAddHideAllApp;
	BOOL	m_isAddShowAllApp;
	UINT m_delayOptimAllApp;
	UINT m_delayOptimAllApp_index;
	UINT m_delayOptimHiddenApp_index;
	UINT m_delayOptimHiddenApp;
	BOOL	m_isReduceProcessMemory;
	BOOL	m_isReduceAllProcessesMemory;
	long	m_wHash;
	BOOL	m_isPwdUseExit;
	BOOL	m_isProgramQuit;
	BOOL	m_isPwdUseSettings;
	BOOL	m_isPwdUseWndSettings;
	BOOL	m_isPwdUseShowMenu;
	BOOL	m_isPwdUseShowPrivateMenu;
	BOOL	m_isPwdUseShowWindow;
	//{{AFX_MSG(CHideApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	HANDLE m_hMutex;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HIDE_H__F484B4B8_1D82_11D6_8D1C_94DA39A0DB44__INCLUDED_)
