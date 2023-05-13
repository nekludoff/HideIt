// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__F484B4BC_1D82_11D6_8D1C_94DA39A0DB44__INCLUDED_)
#define AFX_MAINFRM_H__F484B4BC_1D82_11D6_8D1C_94DA39A0DB44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"
#include "SystemTray.h"
#include "PPageCommon.h"
#include "PPagePerfomance.h"
#include "PPageAppMenu.h"
#include "PPageHide.h"
#include "PPageHotKeys.h"
#include "PPagePasswords.h"
#include "PPageWndMain.h"
#include "PasswordDialog.h"
#include "EnumWinModules.h"
#include "WndParmList.h"
#include "WNDPPageHide.h"
#include "WNDPPagePasswords.h"
#include "WNDPPagePerfomance.h"
#include "InfoStore.h"

#define WM_NOTIFYICONMSG (WM_APP + 1)

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)
	HICON m_hTrayIconEmpty;
	HICON m_hTrayIconFull;
	CMenu m_wndMenu;
	CSystemTray m_SystemTray;

// Attributes
public:

// Operations
public:
	void HideAllWindows(void);
	BOOL m_isMenuDialogActive;
	BOOL WriteSettings(void);
	BOOL ReadSettings(void);
	BOOL m_hasDestroying;
	void HandleAppMenu(HWND hWnd, POINT point, int typeHidden);
	void HandleWndMenu(HWND hWnd, POINT point);
	BOOL HideWindow(HWND hWnd, DWORD event);
	void HandlePopupMenu(HWND hWnd, POINT point);
	int CHideDlg(CWnd* pParent = NULL);	// standard constructor
	int m_lastPropSheetActivePage;
	HWND m_wndMenuHWND;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	enum {
		eventSYSMENU = 0x0001,
		eventMINIMIZE = 0x0002,
		eventNCRBUTTONDOWN = 0x0003,
		eventNCRBUTTONDBLCLK = 0x0004,
		eventHIDEACTIVE = 0x0005,
		eventWNDMENU = 0x0006,
		eventHIDEALL = 0x0007,
	};
	void ProcessWindowSetting(HWND hWnd);
	void ShowWindowSettings(int listIndex);
	void ShowLastHiddenApplication();
	void SetLocalVariables(CPPageCommon* pageCommon, CPPageAppMenu* pageAppMenu, CPPageHide* pageHide, CPPagePerfomance* pagePerfomance, CPPageHotKeys* pageHotKeys, CPPagePasswords* pagePasswords);
	void SetGlobalVariables(CPPageCommon* pageCommon, CPPageAppMenu* pageAppMenu, CPPageHide* pageHide, CPPagePerfomance* pagePerfomance, CPPageHotKeys* pageHotKeys, CPPagePasswords* pagePasswords);
	void SetWNDLocalVariables(int listIndex, CPPageWndMain* pageWndMain, CWNDPPageHide* pageHide, CWNDPPagePerfomance* pagePerfomance, CWNDPPagePasswords* pagePasswords);
	void SetWNDGlobalVariables(int listIndex, CPPageWndMain* pageWndMain, CWNDPPageHide* pageHide, CWNDPPagePerfomance* pagePerfomance, CWNDPPagePasswords* pagePasswords);
	long GetPasswordHash(CString passwordStr);
	void ShowWindowSettings();
	BOOL CheckPassword(long pHash);
	BOOL ShowWindowByHWND(HWND hWnd, int mode);
	BOOL ShowWindowByWndId(DWORD appId);
	int GetWndSettings(HWND hWnd);
	int GetWndSettings(DWORD procId);
	void GetWndIcon(HWND hWnd, HICON& bigWndIcon, HICON& smallWndIcon, BOOL extractExe);
	void HandleContextMenu(HWND hWnd, POINT point);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CChildView    m_wndView;

// Generated message map functions
protected:
	CString m_moduleName;
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnAboutbox();
	afx_msg void OnHideall();
	afx_msg void OnExit();
	afx_msg void OnSettings();
	afx_msg void OnShowall();
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnWndsettings();
	afx_msg void OnHideWnd();
	afx_msg void OnCustomwndsettings();
	afx_msg void OnWndsettingspopup();
	afx_msg void OnSettingspopup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_isHideAllAppActive;
	void CheckForWM_NOTIFYICONMSG(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL m_isHidingActive;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F484B4BC_1D82_11D6_8D1C_94DA39A0DB44__INCLUDED_)
