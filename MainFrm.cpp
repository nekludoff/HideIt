// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "Hide.h"
#include "MainFrm.h"
#include "WndDescr.h"
#include "EZMenu.h"
#include "AboutBox.h"
#include "./WHook/WHook.h"
#include "EnumWnd.h"
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
#include <atlbase.h>
#include "debug.h"
#include "InfoStore.h"
	
#ifdef _DEBUG
#pragma comment(lib,"./Debug/WHook.lib")
#else
#pragma comment(lib,"./Release/WHook.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD itemID;

void CALLBACK EZDrawItemCallBack(LPDRAWITEMSTRUCT lpDrawItem)
{
	itemID = lpDrawItem->itemID;
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(IDM_ABOUTBOX, OnAboutbox)
	ON_COMMAND(IDM_HIDEALL, OnHideall)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_COMMAND(IDM_SETTINGS, OnSettings)
	ON_COMMAND(IDM_SHOWALL, OnShowall)
	ON_WM_QUERYENDSESSION()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_WNDSETTINGS, OnWndsettings)
	ON_COMMAND(IDM_HIDEWND, OnHideWnd)
	ON_COMMAND(IDM_CUSTOMWNDSETTINGS, OnCustomwndsettings)
	ON_COMMAND(IDM_WNDSETTINGSPOPUP, OnWndsettingspopup)
	ON_COMMAND(IDM_SETTINGSPOPUP, OnSettingspopup)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

class CMyPropertySheet :public CPropertySheet
{
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

BOOL CMyPropertySheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == ID_APPLY_NOW))
	{
		CPPagePasswords* ppage = (CPPagePasswords*)GetPage(3);
		if(IsWindow(ppage->GetSafeHwnd()))
		{
			ppage->UpdateData(TRUE);
			if(ppage->m_masterPassword != ppage->m_confirmMasterPassword)
			{
				AfxMessageBox("The password was not correctly confirmed. Please ensure that the password and confirmation match exactly.");
				ppage->m_masterPassword = _T("");
				ppage->m_confirmMasterPassword = _T("");
				ppage->UpdateData(FALSE);
				ppage->m_masterPasswordControl.SetFocus();
				return TRUE;
			}
		}
	}
	//
	if((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
	{
		CMainFrame* mainWnd = (CMainFrame*) AfxGetMainWnd();
		mainWnd->m_lastPropSheetActivePage = GetActiveIndex();
	}
	if((LOWORD(wParam) == ID_APPLY_NOW))
	{
		CMainFrame* mainWnd = (CMainFrame*) AfxGetMainWnd();
		mainWnd->SetGlobalVariables((CPPageCommon*)GetPage(0), (CPPageAppMenu*)GetPage(1), (CPPageHide*)GetPage(2), (CPPagePerfomance*)GetPage(4), (CPPageHotKeys*)GetPage(5), (CPPagePasswords*)GetPage(3));
		mainWnd->WriteSettings();
	}
	return CPropertySheet::OnCommand(wParam, lParam);
}

class CWNDMyPropertySheet :public CPropertySheet
{
public:
	int m_listIndex;
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

BOOL CWNDMyPropertySheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == ID_APPLY_NOW))
	{
		CWNDPPagePasswords* ppage = (CWNDPPagePasswords*)GetPage(2);
		if(IsWindow(ppage->GetSafeHwnd()))
		{
			ppage->UpdateData(TRUE);
			if(ppage->m_masterPassword != ppage->m_confirmMasterPassword)
			{
				AfxMessageBox("The password was not correctly confirmed. Please ensure that the password and confirmation match exactly.");
				ppage->m_masterPassword = _T("");
				ppage->m_confirmMasterPassword = _T("");
				ppage->UpdateData(FALSE);
				ppage->m_masterPasswordControl.SetFocus();
				return TRUE;
			}
		}
	}
	//
	if((LOWORD(wParam) == ID_APPLY_NOW))
	{
		CMainFrame* mainWnd = (CMainFrame*) AfxGetMainWnd();
		mainWnd->SetWNDGlobalVariables(m_listIndex, (CPPageWndMain*) GetPage(0), (CWNDPPageHide*) GetPage(1), (CWNDPPagePerfomance*) GetPage(3), (CWNDPPagePasswords*) GetPage(2));
	}
	return CPropertySheet::OnCommand(wParam, lParam);
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
	m_hasDestroying = FALSE;
	m_isMenuDialogActive = FALSE;
	m_isHidingActive = FALSE;
	m_lastPropSheetActivePage = 0;
	m_moduleName = _T("");
	m_isHideAllAppActive = FALSE;
	m_wndMenuHWND = NULL;
}

CMainFrame::~CMainFrame()
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	//	HotKey Page
	for(int i = 0; i < 10; i ++)
	{
		UnregisterHotKey(this->GetSafeHwnd(), app->m_hotKeyArray[i][2]);
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	m_hTrayIconEmpty = AfxGetApp()->LoadIcon(IDI_TRAYEMPTY);
	m_hTrayIconFull = AfxGetApp()->LoadIcon(IDI_TRAYFULL);
	m_SystemTray.Create(this, WM_NOTIFYICONMSG, "Hide It!",m_hTrayIconEmpty,1);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

long CMainFrame::GetPasswordHash(CString passwordStr)
{
	CString passwordSubStr;
	long	hold = 0;

	for(int i = 0; i < passwordStr.GetLength(); i++)
	{
		int b = passwordStr[0];
		passwordSubStr = passwordStr[i];

		switch((b * (i+1)) % 4)
		{
			case	0:
				hold += (int)passwordStr[i] * (i+1);
				break;
			case	1:
				hold -= (int)passwordStr[i] * (i+1);
				break;
			case	2:
				hold += (int)passwordStr[i] * ((i+1) - (int)passwordStr[i]);
				break;
			case	3:
				hold -= (int)passwordStr[i] * ((i+1) - (int)passwordStr[i]);
				break;
		}

	}
	return hold;
}

BOOL CMainFrame::CheckPassword(long pHash)
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	CPasswordDialog passwordDlg;
	BOOL l_isMenuDialogActive = m_isMenuDialogActive;
	//
	if(m_isMenuDialogActive)
		return FALSE;
	//
	m_isMenuDialogActive = TRUE;
	if(pHash != 0)
	{
		do
		{
			passwordDlg.m_password = _T("");
			int pwdResult = passwordDlg.DoModal();
			if(pwdResult != IDOK)
			{
				m_isMenuDialogActive = l_isMenuDialogActive;
				return FALSE;
			}
			Sleep(200);
		} while (pHash != GetPasswordHash(passwordDlg.m_password));
	}
	m_isMenuDialogActive = l_isMenuDialogActive;
	return TRUE;
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if( ((UINT)wParam > WM_APP+100) & ((UINT)wParam <= WM_APP+250))
		ShowWindowByWndId((DWORD)wParam);

	return CFrameWnd::OnCommand(wParam, lParam);
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	POINT pt;
	CHideApp* app = (CHideApp*) AfxGetApp();
	HWND nWnd;
	CWnd*  fW = CWnd::FromHandle(::GetForegroundWindow());

	switch(message) {
		case WM_NOTIFYICONMSG:
			switch(lParam)  {
				case WM_LBUTTONDOWN:
					GetCursorPos(&pt);
					HandleAppMenu(m_hWnd, pt, 1);
					break;
				case WM_RBUTTONDOWN:
					GetCursorPos(&pt);
					HandlePopupMenu(m_hWnd, pt);
					break;
				default:
					break;
			}
			break;
		case WM_APP+9 :		//	WM_SYSCOMMAND + IDM_MYMENUALL
			HideAllWindows();
			break;
		case WM_APP+10 :	//	WM_SYSCOMMAND + IDM_MYMENU
			HideWindow((HWND) lParam, eventSYSMENU);
			break;
		case WM_APP+11 :	//	WM_REMOVEMENU
			app->m_countHWND--;
			if( (app->m_countHWND < 1) && !m_hasDestroying )
			{
				m_hasDestroying = TRUE;
				if(app->m_isSoftExit)
					PostMessage(WM_COMMAND, (WPARAM) ID_APP_EXIT, 0);
			}
			break;
		case WM_APP+14 :	//	HCBT_MINMAX + SC_MINIMIZED
			HideWindow((HWND) lParam, eventMINIMIZE);
			break;
		case WM_APP+15 :	//	WM_NCRBUTTONDOWN + ( HTMINBUTTON || HTREDUCE )
			HideWindow((HWND) lParam, eventNCRBUTTONDOWN);
			break;
		case WM_APP+18 :	//	WM_NCRBUTTONDBLCLK
			HideWindow((HWND) lParam, eventNCRBUTTONDBLCLK);
			break;
		case WM_APP+21 :	//	WM_NCRBUTTONDBLCLK On HT_ZOOM
			//
			if(app->m_isShowPopupWndMenu)
			{
				if(		((HWND) lParam != app->m_pMainWnd->GetSafeHwnd())
					&&	((HWND) lParam != this->GetSafeHwnd())
					)
					GetCursorPos(&pt);
					HandleWndMenu((HWND) lParam, pt);
			}
			break;
		case WM_APP+30 :	//	local message from application window to read settings
			ReadSettings();
			break;
		case WM_APP+40 :	//	Window not present or it shown
			ShowWindowByHWND((HWND) lParam, 0);
			break;
		case WM_HOTKEY:
			if(!m_isMenuDialogActive)
			{
				if(wParam == (DWORD) app->m_hotKeyArray[0][2])	//	"Window: Hide all windows"
				{
					OnHideall();
				}
				if(wParam == (DWORD) app->m_hotKeyArray[1][2])	//	"Window: Show all windows"
				{
					app->m_typeShowAll = 1;
					OnShowall();
				}
				if(wParam == (DWORD) app->m_hotKeyArray[2][2])	//	"Window: Hide active window"
				{
					nWnd = ::GetForegroundWindow();
					HideWindow(nWnd, eventHIDEACTIVE);
				}
				if(wParam == (DWORD) app->m_hotKeyArray[3][2])	//	"Window: Show last hidden window"
				{
					ShowLastHiddenApplication();
				}
				if(wParam == (DWORD) app->m_hotKeyArray[4][2])	//	"Window: Customize window settings"
				{
					if(		(fW->GetSafeHwnd() != NULL)
						&&	(fW->GetSafeHwnd() != app->m_pMainWnd->GetSafeHwnd())
						&&	(fW->GetSafeHwnd() != this->GetSafeHwnd())
					)
					{
						ProcessWindowSetting(fW->GetSafeHwnd());
					}
				}
				if(wParam == (DWORD) app->m_hotKeyArray[5][2])	//	"Menu: Show system menu"
				{
					GetCursorPos(&pt);
					HandlePopupMenu(m_hWnd, pt);
				}
				if(wParam == (DWORD) app->m_hotKeyArray[6][2])	//	"Menu: Show Hide It! program settings"
				{
					OnSettings();
				}
				if(wParam == (DWORD) app->m_hotKeyArray[7][2])	//	"Menu: Show common list of windows (window menu)"
				{
					GetCursorPos(&pt);
					HandleAppMenu(m_hWnd, pt, 1);
				}
				if(wParam == (DWORD) app->m_hotKeyArray[8][2])	//	"Menu: Show secured private list of windows"
				{
					GetCursorPos(&pt);
					HandleAppMenu(m_hWnd, pt, 0);
				}
				if(wParam == (DWORD) app->m_hotKeyArray[9][2])	//	"Menu: Show private list of windows"
				{
					GetCursorPos(&pt);
					HandleAppMenu(m_hWnd, pt, 2);
				}
				
			}
			break;
		default:
			if(message == app->m_WM_TASKBARCREATED)
			{
				m_SystemTray.Restore(this, WM_NOTIFYICONMSG, "Hide It!",m_hTrayIconEmpty,1);
				m_SystemTray.SetIcon(app->m_globalVisibleWndCount?m_hTrayIconFull:m_hTrayIconEmpty);
			}
			else
			{
				CheckForWM_NOTIFYICONMSG(message, wParam, lParam);
			}
			break;
	}
	return CFrameWnd::WindowProc(message, wParam, lParam);
}


void CMainFrame::CheckForWM_NOTIFYICONMSG(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( (message > WM_APP+100) & (message <= WM_APP+250))
	{
		POINT pt;
		switch(lParam)  {
			case WM_LBUTTONDOWN:
				ShowWindowByWndId(message);
				break;
			case WM_RBUTTONDOWN:
				GetCursorPos(&pt);
				HandleContextMenu(m_hWnd, pt);
				break;
			default:
				break;
		}
	}
}

int CMainFrame::GetWndSettings(DWORD procId)
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	CEnumWinModules em;
	em.Process(0);
	CString progName = em.GetModuleName(procId);

	CWndInfoStore* pWIStore;
	int indexFound = -1;

	for(int i = 0; i < app->m_wndInfoStoreArray.GetSize(); i++)
	{
		pWIStore = (CWndInfoStore*) app->m_wndInfoStoreArray.GetAt(i);
		if(pWIStore->m_progName.CompareNoCase(progName)==0)
		{
			indexFound = i;
			break;
		}
	}
	return indexFound;
}


int CMainFrame::GetWndSettings(HWND hWnd)
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	CWnd* wnd = CWnd::FromHandle(hWnd);
	CEnumWinModules em;
	em.Process(0);
	CString progName = em.GetModuleName(hWnd);

	CWndInfoStore* pWIStore;
	int indexFound = -1;

	for(int i = 0; i < app->m_wndInfoStoreArray.GetSize(); i++)
	{
		pWIStore = (CWndInfoStore*) app->m_wndInfoStoreArray.GetAt(i);
		if(pWIStore->m_progName.CompareNoCase(progName)==0)
		{
			CString wndName;
			wnd->GetWindowText(wndName);
			if(wndName.Find(pWIStore->m_wndName,0) > -1)
			{
				indexFound = i;
				break;
			}
		}
	}
	return indexFound;
}

void CMainFrame::HandleWndMenu(HWND hWnd, POINT point)
{
	HMENU hMenu;
	HMENU hMenuTrackPopup;
	LPCSTR pszIDMenu;
	BOOL isAutoMenu = m_bAutoMenuEnable;

	if(m_isMenuDialogActive)
	{
		return;
	}
	m_wndMenuHWND = hWnd;
	m_bAutoMenuEnable = FALSE;
	SetForegroundWindow();
	pszIDMenu = MAKEINTRESOURCE(IDM_WNDMENU);
	hMenu = LoadMenu(AfxGetInstanceHandle(), pszIDMenu);
	if (!hMenu)  return;
	hMenuTrackPopup = GetSubMenu(hMenu, 0);
	if (!hMenuTrackPopup)  return;
	TrackPopupMenu(hMenuTrackPopup, TPM_LEFTALIGN, point.x, point.y, 0, m_hWnd, NULL);
	PostMessage(WM_NULL, 0, 0);
	DestroyMenu (hMenu);
	m_bAutoMenuEnable = isAutoMenu;
}


void CMainFrame::HandlePopupMenu(HWND hWnd, POINT point)
{
	HMENU hMenu;
	HMENU hMenuTrackPopup;
	LPCSTR pszIDMenu;
	BOOL isAutoMenu = m_bAutoMenuEnable;

	if(m_isMenuDialogActive)
	{
		return;
	}
	m_bAutoMenuEnable = FALSE;
	SetForegroundWindow();
	pszIDMenu = MAKEINTRESOURCE(IDM_POPUPMENUICON);
	hMenu = LoadMenu(AfxGetInstanceHandle(), pszIDMenu);
	if (!hMenu)  return;
	hMenuTrackPopup = GetSubMenu(hMenu, 0);
	if (!hMenuTrackPopup)  return;

	if(m_isMenuDialogActive)
	{
		::EnableMenuItem(hMenuTrackPopup, IDM_ABOUTBOX, MF_BYCOMMAND | MF_GRAYED);
		::EnableMenuItem(hMenuTrackPopup, IDM_SETTINGS, MF_BYCOMMAND | MF_GRAYED);
		::EnableMenuItem(hMenuTrackPopup, IDM_EXIT, MF_BYCOMMAND | MF_GRAYED);
	} else
	{
		::EnableMenuItem(hMenuTrackPopup, IDM_ABOUTBOX, MF_BYCOMMAND | MF_ENABLED);
		::EnableMenuItem(hMenuTrackPopup, IDM_SETTINGS, MF_BYCOMMAND | MF_ENABLED);
		::EnableMenuItem(hMenuTrackPopup, IDM_EXIT, MF_BYCOMMAND | MF_ENABLED);
		SetMenuDefaultItem(hMenuTrackPopup, IDM_EXIT, FALSE);
	}
	TrackPopupMenu(hMenuTrackPopup, TPM_LEFTALIGN, point.x, point.y, 0, hWnd, NULL);
	PostMessage(WM_NULL, 0, 0);
	DestroyMenu (hMenu);
	m_bAutoMenuEnable = isAutoMenu;
}

void CMainFrame::OnExit() 
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	if(app->m_isPwdUseExit && !CheckPassword(app->m_wHash))
	{
		m_isMenuDialogActive = FALSE;
		return;
	}
	app->m_wndStore.WriteInfo(app->m_wndInfoStoreFileName);
	app->ExitInstanceProcess();
	app->m_isPwdUseShowWindow = FALSE;
	app->m_isProgramQuit = TRUE;
	app->m_typeShowAll = -1;
	OnShowall();
	app->ModifyAllWindows(FALSE);
	if(!app->m_isSoftExit)
		PostMessage(WM_COMMAND, (WPARAM) ID_APP_EXIT, 0);
}

void CMainFrame::OnAboutbox() 
{
	if(m_isMenuDialogActive)
	{
		return;
	}
	m_isMenuDialogActive = TRUE;
	CAboutBox dlgAbout;
	dlgAbout.DoModal();
	m_isMenuDialogActive = FALSE;
}

void CMainFrame::OnHideWnd()
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	if(m_isMenuDialogActive)
	{
		return;
	}
	m_isMenuDialogActive = TRUE;
	HideWindow(m_wndMenuHWND, eventWNDMENU);
	m_wndMenuHWND = NULL;
	m_isMenuDialogActive = FALSE;
}

void CMainFrame::OnWndsettings() 
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	if(m_isMenuDialogActive)
	{
		return;
	}
	if(app->m_isPwdUseWndSettings && !CheckPassword(app->m_wHash))
	{
		return;
	}
	m_isMenuDialogActive = TRUE;
	CWndParmList wndParmList;
	wndParmList.DoModal();
	m_isMenuDialogActive = FALSE;
}

void CMainFrame::OnCustomwndsettings() 
{
	ProcessWindowSetting(m_wndMenuHWND);
}


void CMainFrame::ProcessWindowSetting(HWND hWnd)
{
	HICON	bigWndIcon = NULL;
	HICON	smallWndIcon = NULL;
	int		indexFound = -1;
	CHideApp* app = (CHideApp*) AfxGetApp();
	CWnd* wnd = CWnd::FromHandle(hWnd);
	CWndInfoStore*	pWndInfoStore;
	CWndInfoStore*	pWIStore;

	if(hWnd == NULL)
		return;
	if(m_isMenuDialogActive)
	{
		return;
	}
	m_isMenuDialogActive = TRUE;

	pWndInfoStore = new CWndInfoStore();
	wnd->GetWindowText(pWndInfoStore->m_wndName);

	CString str = "Are you sure customize settings for window: \n\n\"" + pWndInfoStore->m_wndName + "\"?";
	if(::MessageBox(::GetDesktopWindow(), str,"Hide It!", MB_YESNO|MB_ICONQUESTION|MB_SYSTEMMODAL) == IDNO)
	{
		m_isMenuDialogActive = FALSE;
		delete pWndInfoStore;
		return;
	}

	m_isMenuDialogActive = FALSE;
	if(app->m_isPwdUseWndSettings && !CheckPassword(app->m_wHash))
	{
		m_isMenuDialogActive = FALSE;
		delete pWndInfoStore;
		return;
	}

	m_isMenuDialogActive = TRUE;
	CEnumWinModules em;
	em.Process(0);
	pWndInfoStore->m_progName = em.GetModuleName(hWnd);

	for(int i = 0; i < app->m_wndInfoStoreArray.GetSize(); i++)
	{
		pWIStore = (CWndInfoStore*) app->m_wndInfoStoreArray.GetAt(i);
		if(pWIStore->m_progName == pWndInfoStore->m_progName)
		{
			if(pWndInfoStore->m_wndName.Find(pWIStore->m_wndName,0) > -1)
			{
				indexFound = i;
				break;
			}
		}
	}
	if(indexFound == -1)
	{
		pWndInfoStore->m_isHideWhenMinimize = app->m_isHideWhenMinimize;
		pWndInfoStore->m_isHideWhenCreated = app->m_isHideWhenCreated;
		pWndInfoStore->m_isHideByRMouse = app->m_isHideByRMouse;
		pWndInfoStore->m_isHideByDblRMouse = app->m_isHideByDblRMouse;
		pWndInfoStore->m_hideMethod = app->m_hideMethod;
		pWndInfoStore->m_isShowOnActivateApp = app->m_isShowOnActivateApp;
		pWndInfoStore->m_hideTo = app->m_hideTo;
		pWndInfoStore->m_isActivateNextWindow = app->m_isActivateNextWindow;
		pWndInfoStore->m_wHash = 0;
		pWndInfoStore->m_isPwdUseShowWindow = app->m_isPwdUseShowWindow;
		pWndInfoStore->m_isChangeProcessPriority = app->m_isChangeProcessPriority;
		pWndInfoStore->m_processPriority = app->m_processPriority;
		pWndInfoStore->m_isDontChangeProcessPriorityPM = app->m_isDontChangeProcessPriorityPM;
		pWndInfoStore->m_isReduceProcessMemory = app->m_isReduceProcessMemory;

		indexFound = app->m_wndInfoStoreArray.Add(pWndInfoStore);
		GetWndIcon(wnd->GetSafeHwnd(), bigWndIcon, smallWndIcon, TRUE);
		if(bigWndIcon == NULL)
			bigWndIcon = app->LoadIcon(IDI_EMPTY);
		app->m_wndInfoStoreImageList.Add(smallWndIcon);
	}
	else
		delete pWndInfoStore;
	//
	ShowWindowSettings(indexFound);
	wnd->SetForegroundWindow();
	m_isMenuDialogActive = FALSE;
}

void CMainFrame::ShowWindowSettings()
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	HWND hWndFore = ::GetForegroundWindow();
	ShowWindowSettings(0);
}

void CMainFrame::ShowWindowSettings(int listIndex)
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	HWND hWndFore = ::GetForegroundWindow();
	CWNDMyPropertySheet	PropSheet;
	CPPageWndMain		pageWndMain;
	CWNDPPageHide			pageHide;
	CWNDPPagePerfomance		pagePerfomance;
	CWNDPPagePasswords		pagePasswords;

	pageWndMain.m_wndListIndex = listIndex;
	SetWNDLocalVariables(listIndex, &pageWndMain, &pageHide, &pagePerfomance, &pagePasswords);

	PropSheet.AddPage(&pageWndMain);
	PropSheet.AddPage(&pageHide);
	PropSheet.AddPage(&pagePasswords);
	PropSheet.AddPage(&pagePerfomance);
	PropSheet.SetTitle(_T("Window Settings "));
	PropSheet.m_listIndex = listIndex;

	int propResult = PropSheet.DoModal();
	if(propResult == IDOK)
	{
		SetWNDGlobalVariables(listIndex, &pageWndMain, &pageHide, &pagePerfomance, &pagePasswords);
	}
	::SetForegroundWindow(hWndFore);
}

void CMainFrame::SetWNDGlobalVariables(int listIndex, CPPageWndMain* pageWndMain, CWNDPPageHide* pageHide, CWNDPPagePerfomance* pagePerfomance, CWNDPPagePasswords* pagePasswords)
{

	CHideApp* app = (CHideApp*) AfxGetApp();
	CWndInfoStore*	pWIStore = (CWndInfoStore*) app->m_wndInfoStoreArray.GetAt(listIndex);
	//	Main Page
	pWIStore->m_wndName = pageWndMain->m_wndName;
	pWIStore->m_progName = pageWndMain->m_wndExeName;
	//	Hide Page
	pWIStore->m_isHideWhenMinimize = pageHide->m_isHideWhenMinimize;
	pWIStore->m_isHideByRMouse = pageHide->m_isHideByRMouse;
	pWIStore->m_isHideByDblRMouse = pageHide->m_isHideByDblRMouse;
	pWIStore->m_hideMethod = pageHide->m_hideMethod;
	pWIStore->m_isShowOnActivateApp = pageHide->m_isShowOnActivateApp;
	pWIStore->m_hideTo = pageHide->m_hideTo;
	pWIStore->m_isActivateNextWindow = pageHide->m_isActivateNextWindow;
	pWIStore->m_isHideWhenCreated = pageHide->m_isHideWhenCreated;
	//	Password Page
	if(	(pagePasswords->m_masterPassword != pagePasswords->m_oldMasterPassword) &&
		(pagePasswords->m_masterPassword == pagePasswords->m_confirmMasterPassword))
		pWIStore->m_wHash = GetPasswordHash(pagePasswords->m_masterPassword);
	//	Perfomance Page (some values only for NT based system)
	pWIStore->m_isChangeProcessPriority = pagePerfomance->m_isChangeProcessPriority;
	pWIStore->m_processPriority = pagePerfomance->m_processPriority;
	pWIStore->m_isDontChangeProcessPriorityPM = pagePerfomance->m_isDontChangeProcessPriorityPM;
}

void CMainFrame::SetWNDLocalVariables(int listIndex, CPPageWndMain* pageWndMain, CWNDPPageHide* pageHide, CWNDPPagePerfomance* pagePerfomance, CWNDPPagePasswords* pagePasswords)
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	CWndInfoStore*	pWIStore = (CWndInfoStore*) app->m_wndInfoStoreArray.GetAt(listIndex);
	//	Main Page
	pageWndMain->m_wndName = pWIStore->m_wndName;
	pageWndMain->m_wndExeName = pWIStore->m_progName;
	//	Hide Page
	pageHide->m_isHideWhenMinimize = pWIStore->m_isHideWhenMinimize;
	pageHide->m_isHideByRMouse = pWIStore->m_isHideByRMouse;
	pageHide->m_isHideByDblRMouse = pWIStore->m_isHideByDblRMouse;
	pageHide->m_hideMethod = pWIStore->m_hideMethod;
	pageHide->m_isShowOnActivateApp = pWIStore->m_isShowOnActivateApp;
	pageHide->m_hideTo = pWIStore->m_hideTo;
	pageHide->m_isActivateNextWindow = pWIStore->m_isActivateNextWindow;
	pageHide->m_isHideWhenCreated = pWIStore->m_isHideWhenCreated;
	//	Password Page
	if(pWIStore->m_wHash == 0)
	{
		pagePasswords->m_masterPassword = _T("");
		pagePasswords->m_confirmMasterPassword = _T("");
	}
	//	Perfomance Page (some values only for NT based system)
	pagePerfomance->m_isChangeProcessPriority = pWIStore->m_isChangeProcessPriority;
	pagePerfomance->m_processPriority = pWIStore->m_processPriority;
	pagePerfomance->m_isDontChangeProcessPriorityPM = pWIStore->m_isDontChangeProcessPriorityPM;
}

void CMainFrame::GetWndIcon(HWND hWnd, HICON& bigWndIcon, HICON& smallWndIcon, BOOL extractExe)
{
	if(bigWndIcon == NULL || smallWndIcon == NULL)
	{
		bigWndIcon = (HICON)::SendMessage(hWnd, WM_QUERYDRAGICON, 0, 0);
		if(bigWndIcon == NULL)
		{
			bigWndIcon = (HICON)::GetClassLong(hWnd, GCL_HICON);
			smallWndIcon = (HICON)::GetClassLong(hWnd, GCL_HICONSM);
		}
		if(bigWndIcon == NULL)
			bigWndIcon = (HICON)::SendMessage(hWnd, WM_GETICON, ICON_BIG, 0);
		if(smallWndIcon == NULL)
			smallWndIcon = (HICON)::SendMessage(hWnd, WM_GETICON, ICON_SMALL, 0);
	}
	//
	if(bigWndIcon == NULL || smallWndIcon == NULL)
	{
		bigWndIcon = (HICON)::SendMessage(hWnd, WM_QUERYDRAGICON, 0, 0);
		smallWndIcon = bigWndIcon;
	}
	if(bigWndIcon == NULL || smallWndIcon == NULL || hWnd == NULL)
	{
		if(extractExe)
		{
			CEnumWinModules em;
			HICON hIcon;

			em.Process(0);
			if(bigWndIcon == NULL)
			{
				::ExtractIconEx(em.GetModuleName(hWnd), 0, &bigWndIcon, &hIcon, 1);
			}
			if(smallWndIcon == NULL)
			{
				::ExtractIconEx(em.GetModuleName(hWnd), 0, &hIcon, &smallWndIcon, 1);
			}
		}
	}
	if(bigWndIcon == NULL)
		bigWndIcon = smallWndIcon;
}

BOOL CMainFrame::HideWindow(HWND hWnd, DWORD event)
{
	static long lCount = 0;
	CHideApp* app = (CHideApp*) AfxGetApp();
	CWnd* wnd = CWnd::FromHandle(hWnd);
	HWND mainWnd = app->m_pMainWnd->GetSafeHwnd();
	DWORD lpdwProcessId, lpdwThreadId;
	BOOL isFound = FALSE;
	int wndId;
	int priorityClass;
	LPWNDDESCR wndDescr;
	CString iconText;
	CString progName = "";

	if(!IsWindow(hWnd)) return FALSE;

	if(!::IsWindowVisible(hWnd))
		return FALSE;
	
	// Prevent to Hide an all child window
	lpdwThreadId = GetWindowThreadProcessId(hWnd, &lpdwProcessId);
	if( (hWnd == mainWnd) || (GetCurrentProcessId() == lpdwProcessId) )
	{	
		return FALSE;
	}

	if(app->m_userName.IsEmpty())
	{
		lCount++;
		if((lCount % 25) == 0)
		{
			lCount = 0;
			AfxMessageBox("Unregistered Version.\n   Register please.");
		}
	}

	// Lock for prevent access
	CSingleLock csl(&app->m_CSList);
	csl.Lock();
	for(int lpos = 0; lpos < app->m_wndArray.GetSize(); lpos++)
	{
		LPWNDDESCR wndDescr = (LPWNDDESCR) app->m_wndArray.GetAt(lpos);
		if(hWnd == wndDescr->m_hWnd)
		{
			if(IsWindow(hWnd))
			{
				if(::IsWindowVisible(hWnd))
				{
					::ShowWindow(hWnd, SW_HIDE);
				}
			}
			csl.Unlock();
			return FALSE;
		}
	}

	BOOL	l_isHideWhenMinimize = app->m_isHideWhenMinimize;
	BOOL	l_isHideWhenCreated = app->m_isHideWhenCreated;
	BOOL	l_isHideByRMouse = app->m_isHideByRMouse;
	BOOL	l_isHideByDblRMouse = app->m_isHideByDblRMouse;
	int		l_hideMethod = app->m_hideMethod;
	BOOL	l_isShowOnActivateApp = app->m_isShowOnActivateApp;
	int		l_hideTo = app->m_hideTo;
	BOOL	l_isActivateNextWindow = app->m_isActivateNextWindow;
	long	l_wHash = app->m_wHash;
	BOOL	l_isPwdUseShowWindow = app->m_isPwdUseShowWindow;
	BOOL	l_isChangeProcessPriority = app->m_isChangeProcessPriority;
	int		l_processPriority = app->m_processPriority;
	BOOL	l_isDontChangeProcessPriorityPM = app->m_isDontChangeProcessPriorityPM;
	BOOL	l_isReduceProcessMemory = app->m_isReduceProcessMemory;

	//
	CString	wndText;
	wnd->GetWindowText(wndText);
	int indexFound = -1;
	
	CEnumWinModules em;
	em.Process(0);
	progName = em.GetModuleName(hWnd);
	CWndInfoStore* pWIStore;

	for(int i = 0; i < app->m_wndInfoStoreArray.GetSize(); i++)
	{
		pWIStore = (CWndInfoStore*) app->m_wndInfoStoreArray.GetAt(i);
		if(pWIStore->m_progName == progName)
		{
			if(wndText.Find(pWIStore->m_wndName,0) > -1)
			{
				indexFound = i;
				break;
			}
		}
	}
	
	if(indexFound > -1)
	{
		app->m_isHideWhenMinimize = pWIStore->m_isHideWhenMinimize;
		app->m_isHideWhenCreated = pWIStore->m_isHideWhenCreated;
		app->m_isHideByRMouse = pWIStore->m_isHideByRMouse;
		app->m_isHideByDblRMouse = pWIStore->m_isHideByDblRMouse;
		app->m_hideMethod = pWIStore->m_hideMethod;
		app->m_isShowOnActivateApp = pWIStore->m_isShowOnActivateApp;
		app->m_hideTo = pWIStore->m_hideTo;
		app->m_isActivateNextWindow = pWIStore->m_isActivateNextWindow;
		app->m_wHash = pWIStore->m_wHash;
		app->m_isPwdUseShowWindow = pWIStore->m_isPwdUseShowWindow;
		app->m_isChangeProcessPriority = pWIStore->m_isChangeProcessPriority;
		app->m_processPriority = pWIStore->m_processPriority;
		app->m_isDontChangeProcessPriorityPM = pWIStore->m_isDontChangeProcessPriorityPM;
		app->m_isReduceProcessMemory = pWIStore->m_isReduceProcessMemory;
	}
	//
	//
	switch(event)
	{
		case eventMINIMIZE:
			if(!app->m_isHideWhenMinimize)
				goto exit;
				break;
		case eventNCRBUTTONDOWN:
			if(!app->m_isHideByRMouse)
				goto exit;
				break;
		case eventNCRBUTTONDBLCLK:
			if(!app->m_isHideByDblRMouse)
				goto exit;
				break;
		default:
				break;
	}
	//
	wndDescr = new CWndDescr;
	wndDescr->m_programName = progName;
	wnd->GetWindowText(wndDescr->m_windowText);
	iconText = wndDescr->m_windowText;
	wndDescr->m_hWnd = hWnd;
	wndDescr->m_isMaster = 1;
	wndDescr->m_hWndParent = ::GetParent(hWnd);
	wndId = app->GetMinWndId();
	wndDescr->m_wndId = wndId;
	if(hWnd == ::GetForegroundWindow())
		wndDescr->m_foregroundWindow = hWnd;
	else
		wndDescr->m_foregroundWindow = NULL;
	wndDescr->m_processId = lpdwProcessId;
	wndDescr->m_threadId = lpdwThreadId; 
	wndDescr->m_createTime = CTime::GetCurrentTime();
	::GetWindowRect(hWnd, &wndDescr->m_wndRect);
	if(app->m_isTrimCaption)
	{
		int i = atoi(app->m_trimCaptionChars);
		wndDescr->m_windowText = wndDescr->m_windowText.Left(i);
	}
	GetWndIcon(hWnd, wndDescr->m_bigIcon, wndDescr->m_smallIcon, app->m_isExtractIconFromFile);
	//
	//	Set PriorityClass
	//	set priority only for one window of process
	for(lpos = 0; lpos < app->m_wndArray.GetSize(); lpos++)
	{
		LPWNDDESCR lwndDescr = (LPWNDDESCR) app->m_wndArray.GetAt(lpos);
		if( (lwndDescr->m_processId == lpdwProcessId) && lwndDescr->m_isPriorityChanged )
		{
			isFound = TRUE;
			break;
		}
	}
	if(!isFound )
	{
		HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, lpdwProcessId);
		if(hProc != NULL)
			wndDescr->m_priorityClass = GetPriorityClass(hProc);
		else
			wndDescr->m_priorityClass = 0;
		if(app->m_isChangeProcessPriority)
		{
			if(! ((app->m_PMProcessId == lpdwProcessId) && app->m_isDontChangeProcessPriorityPM) )
			{
				switch(app->m_processPriority)
				{
					case 0: 
						SetPriorityClass(hProc, HIGH_PRIORITY_CLASS);
						wndDescr->m_isPriorityChanged = TRUE;
						break;
					case 1: 
						SetPriorityClass(hProc, NORMAL_PRIORITY_CLASS);
						wndDescr->m_isPriorityChanged = TRUE;
						break;
					case 2: 
						SetPriorityClass(hProc, IDLE_PRIORITY_CLASS);
						wndDescr->m_isPriorityChanged = TRUE;
						break;
					default :
						break;
				}
			}
		}
		if(app->m_osi.IsNT())
			if(app->m_isReduceProcessMemory)
				SetProcessWorkingSetSize(hProc, -1, -1);
		CloseHandle(hProc);
	}
	else
		wndDescr->m_priorityClass = 0;
	priorityClass = wndDescr->m_priorityClass;
	//
	//	Is hide to Tray?
	if(iconText.GetLength() > 63)
		iconText = iconText.Left(59) + _T("... ");
	//
	switch(app->m_hideTo)
	{
		case 2:
			{
				wndDescr->m_typeHidden = 0;
				app->m_globalInVisibleWndCount++;
				break;
			}

		case 3:
			{
				wndDescr->m_typeHidden = 2;
				app->m_globalVisibleInVisibleWndCount++;
				break;
			}
		default:
			{
				HICON hicon = wndDescr->m_smallIcon?wndDescr->m_smallIcon:wndDescr->m_bigIcon;
				ICONINFO iconInfo;
				BOOL success = GetIconInfo(hicon, &iconInfo);
				if((app->m_hideTo == 1) && success)
					wndDescr->m_SystemTray.Create(this, wndDescr->m_wndId, iconText, wndDescr->m_smallIcon?wndDescr->m_smallIcon:wndDescr->m_bigIcon, wndDescr->m_wndId);
				//
				wndDescr->m_typeHidden = 1;
				app->m_globalVisibleWndCount++;
				break;
			}
	}
	//
	INFOSTORE iStore;
	iStore.dwInfoId = wndDescr->m_wndId;
	iStore.hWnd = hWnd;
	iStore.dwState = 1;
	iStore.dwProcessId = lpdwProcessId;
	iStore.dwThreadId = lpdwThreadId;
	iStore.dwPriority = wndDescr->m_priorityClass;
	app->m_infoStore.WriteInfo(iStore);
	//
	::ShowWindow(hWnd, SW_HIDE);
	app->m_wndArray.Add(wndDescr);
	m_SystemTray.SetIcon(app->m_globalVisibleWndCount?m_hTrayIconFull:m_hTrayIconEmpty);
	///////////////////////////////////
	// Is hide all application windows?
	if(	(app->m_hideMethod == 1) || (app->m_hideMethod == 2) || (app->m_hideMethod == 3) )
	{
		CEnumWnd enumWnd;
		if(enumWnd.EnumWnd(CEnumWnd::enumSKIPINVISIBLE, lpdwProcessId))
		{
			for (int i=0;i < enumWnd.m_wndInfoArray.GetSize();i++)
			{
				LPWNDINFO wndInfo = (LPWNDINFO) enumWnd.m_wndInfoArray.ElementAt(i);
				
				if (!IsWindow(hWnd)) 
					continue;
				if(!::IsWindowVisible(wndInfo->hWnd))
					continue;

				// Is hide all dependents windows?
				if(	app->m_hideMethod == 1 )
				{
					BOOL isChild = ::IsChild(hWnd, wndInfo->hWnd);
					BOOL isOwner =  wndInfo->hWnd == ::GetWindow(hWnd, GW_OWNER);
					isOwner =  isOwner || hWnd == ::GetWindow(wndInfo->hWnd, GW_OWNER);

					if(!isChild && !isOwner)
						continue;
				}
				
				LPWNDDESCR wndDescr = new CWndDescr;

				wndDescr->m_hWnd = wndInfo->hWnd;
				wndDescr->m_wndId = wndId;
				wndDescr->m_hWndParent = ::GetParent(wndInfo->hWnd);
				wndDescr->m_processId = wndInfo->processId;
				wndDescr->m_threadId = wndInfo->threadId;
				wndDescr->m_createTime = CTime::GetCurrentTime();
				wndDescr->m_windowText = wndInfo->wText;
				if(app->m_isTrimCaption)
				{
					int i = atoi(app->m_trimCaptionChars);
					wndDescr->m_windowText = wndDescr->m_windowText.Left(i);
				}
				GetWndIcon(wndInfo->hWnd, wndDescr->m_bigIcon, wndDescr->m_smallIcon, app->m_isExtractIconFromFile);
				::GetWindowRect(wndInfo->hWnd, &wndDescr->m_wndRect);
				switch(app->m_hideTo)
				{
					case 2:
						{
							wndDescr->m_typeHidden = 0;
							app->m_globalInVisibleWndCount++;
							break;
						}

					case 3:
						{
							wndDescr->m_typeHidden = 2;
							app->m_globalVisibleInVisibleWndCount++;
							break;
						}
					default:
						{
							HICON hicon = wndDescr->m_smallIcon?wndDescr->m_smallIcon:wndDescr->m_bigIcon;
							ICONINFO iconInfo;
							BOOL success = GetIconInfo(hicon, &iconInfo);
							if((app->m_hideTo == 1) && success)
							//	wndDescr->m_SystemTray.Create(this, wndDescr->m_wndId, iconText, wndDescr->m_smallIcon?wndDescr->m_smallIcon:wndDescr->m_bigIcon, wndDescr->m_wndId);
							//
							wndDescr->m_typeHidden = 1;
							app->m_globalVisibleWndCount++;
							break;
						}
				}
				wndDescr->m_isMaster = 0;
				app->m_wndArray.Add(wndDescr);
				::ShowWindow(wndInfo->hWnd, SW_HIDE);
				iStore.dwInfoId = wndId;
				iStore.hWnd = wndInfo->hWnd;
				iStore.dwState = 1;
				iStore.dwProcessId = lpdwProcessId;
				iStore.dwThreadId = wndInfo->threadId;
				iStore.dwPriority = priorityClass;
				app->m_infoStore.WriteInfo(iStore);
			}
		}
	}
	//
	//
	if(app->m_isActivateNextWindow && !m_isHideAllAppActive)
	{
		// Simulate a key press
		keybd_event(VK_MENU, 0, 0, 0);
		keybd_event(VK_TAB, 0, 0, 0);
		// Simulate a key release
		keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
	}
exit:
	app->m_isHideWhenMinimize = l_isHideWhenMinimize;
	app->m_isHideWhenCreated = l_isHideWhenCreated;
	app->m_isHideByRMouse = l_isHideByRMouse;
	app->m_isHideByDblRMouse = l_isHideByDblRMouse;
	app->m_hideMethod = l_hideMethod;
	app->m_isShowOnActivateApp = l_isShowOnActivateApp;
	app->m_hideTo = l_hideTo;
	app->m_isActivateNextWindow = l_isActivateNextWindow;
	app->m_wHash = l_wHash;
	app->m_isPwdUseShowWindow = l_isPwdUseShowWindow;
	app->m_isChangeProcessPriority = l_isChangeProcessPriority;
	app->m_processPriority = l_processPriority;
	app->m_isDontChangeProcessPriorityPM = l_isDontChangeProcessPriorityPM;
	app->m_isReduceProcessMemory = l_isReduceProcessMemory;
	csl.Unlock();
	return TRUE;
}

BOOL CMainFrame::ShowWindowByWndId(DWORD wndId)
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	LPWNDDESCR wndDescr;
	CWndInfoStore* pWIStore;
	int indexFound = -1;

	BOOL l_isPwdUseShowWindow = FALSE;
	long l_wHash = 0;


	CSingleLock csl(&app->m_CSList);
	csl.Lock();
	for(int pos = app->m_wndArray.GetUpperBound(); pos >= 0; pos--)
	{
		wndDescr = (LPWNDDESCR) app->m_wndArray.GetAt(pos);
		if( (wndId == wndDescr->m_wndId) && (wndDescr->m_isMaster == 1) )
		{
			if(!app->m_isProgramQuit)
			{
				for(int i = 0; i < app->m_wndInfoStoreArray.GetSize(); i++)
				{
					pWIStore = (CWndInfoStore*) app->m_wndInfoStoreArray.GetAt(i);
					if(pWIStore->m_progName == wndDescr->m_programName)
					{
						if(wndDescr->m_windowText.Find(pWIStore->m_wndName,0) > -1)
						{
							indexFound = i;
							break;
						}
					}
				}
				if(indexFound > -1)
				{
					l_wHash = pWIStore->m_wHash;
					l_isPwdUseShowWindow = l_wHash?1:0;
				}
				else
				{
					l_wHash = app->m_wHash;
					l_isPwdUseShowWindow = app->m_isPwdUseShowWindow;
				}
				//			
				if(l_isPwdUseShowWindow && !CheckPassword(l_wHash))
				{
					m_isMenuDialogActive = FALSE;
					csl.Unlock();
					return FALSE;
				}
			}
			//
			app->m_wndArray.RemoveAt(pos);
			// Set PriorityClass
			if(wndDescr->m_priorityClass != 0)
			{
				if(wndDescr->m_isMaster == 1)
				{
					HANDLE hProc = OpenProcess(PROCESS_SET_INFORMATION, FALSE, wndDescr->m_processId);
					if(hProc != NULL)
						SetPriorityClass(hProc, wndDescr->m_priorityClass);
					CloseHandle(hProc);
				}
			}
			//
			if(IsWindow(wndDescr->m_hWnd))
			{
				::ShowWindow(wndDescr->m_hWnd, SW_SHOW);
				::SetForegroundWindow(wndDescr->m_hWnd);
			}
			//
			switch(wndDescr->m_typeHidden)
			{
				case	0:	app->m_globalInVisibleWndCount--;
							break;			
				case	1:	app->m_globalVisibleWndCount--;
							break;
				case	2:	app->m_globalVisibleInVisibleWndCount--;
							break;

			}
			//
			INFOSTORE iStore;
			iStore.dwInfoId = wndDescr->m_wndId;
			iStore.hWnd = wndDescr->m_hWnd;
			iStore.dwState = 0;
			iStore.dwProcessId = wndDescr->m_processId;
			iStore.dwThreadId = wndDescr->m_threadId;
			iStore.dwPriority = wndDescr->m_priorityClass;
			app->m_infoStore.WriteInfo(iStore);
			//
			if(wndDescr->m_foregroundWindow != NULL)
				::SetForegroundWindow(wndDescr->m_foregroundWindow);
			app->ReleaseWndId(wndDescr->m_wndId);
			//
			delete wndDescr;
		}
	}
	//
	indexFound = -1;
	l_isPwdUseShowWindow = FALSE;
	l_wHash = 0;
	//
	for(pos = app->m_wndArray.GetUpperBound(); pos >= 0; pos--)
	{
		wndDescr = (LPWNDDESCR) app->m_wndArray.GetAt(pos);
		if( (wndId == wndDescr->m_wndId) && (wndDescr->m_isMaster == 0) )
		{
			if(!app->m_isProgramQuit)
			{
				for(int i = 0; i < app->m_wndInfoStoreArray.GetSize(); i++)
				{
					pWIStore = (CWndInfoStore*) app->m_wndInfoStoreArray.GetAt(i);
					if(pWIStore->m_progName == wndDescr->m_programName)
					{
						if(wndDescr->m_windowText.Find(pWIStore->m_wndName,0) > -1)
						{
							indexFound = i;
							break;
						}
					}
				}
				if(indexFound > -1)
				{
					l_wHash = pWIStore->m_wHash;
					l_isPwdUseShowWindow = l_wHash?1:0;
				}
				else
				{
					l_wHash = app->m_wHash;
					l_isPwdUseShowWindow = app->m_isPwdUseShowWindow;
				}
				//			
				if(l_isPwdUseShowWindow && !CheckPassword(l_wHash))
				{
					m_isMenuDialogActive = FALSE;
					csl.Unlock();
					return FALSE;
				}
			}
			//
			app->m_wndArray.RemoveAt(pos);
			// Set PriorityClass
			if(wndDescr->m_priorityClass != 0)
			{
				if(wndDescr->m_isMaster == 1)
				{
					HANDLE hProc = OpenProcess(PROCESS_SET_INFORMATION, FALSE, wndDescr->m_processId);
					if(hProc != NULL)
						SetPriorityClass(hProc, wndDescr->m_priorityClass);
					CloseHandle(hProc);
				}
			}
			//
			if(IsWindow(wndDescr->m_hWnd))
			{
				::ShowWindow(wndDescr->m_hWnd, SW_SHOW);
				::SetForegroundWindow(wndDescr->m_hWnd);
			}
			//
			switch(wndDescr->m_typeHidden)
			{
				case	0:	app->m_globalInVisibleWndCount--;
							break;			
				case	1:	app->m_globalVisibleWndCount--;
							break;
				case	2:	app->m_globalVisibleInVisibleWndCount--;
							break;

			}
			//
			INFOSTORE iStore;
			iStore.dwInfoId = wndDescr->m_wndId;
			iStore.hWnd = wndDescr->m_hWnd;
			iStore.dwState = 0;
			iStore.dwProcessId = wndDescr->m_processId;
			iStore.dwThreadId = wndDescr->m_threadId;
			iStore.dwPriority = wndDescr->m_priorityClass;
			app->m_infoStore.WriteInfo(iStore);
			//
			if(wndDescr->m_foregroundWindow != NULL)
				::SetForegroundWindow(wndDescr->m_foregroundWindow);
			app->ReleaseWndId(wndDescr->m_wndId);
			//
			delete wndDescr;
		}
	}
	//
	m_SystemTray.SetIcon(app->m_globalVisibleWndCount?m_hTrayIconFull:m_hTrayIconEmpty);
	csl.Unlock();
	return TRUE;
}

BOOL CMainFrame::ShowWindowByHWND(HWND hWnd, int mode)
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	LPWNDDESCR wndDescr;
	int isFound = -1;

	CSingleLock csl(&app->m_CSList);
	csl.Lock();
	for(int pos = app->m_wndArray.GetUpperBound(); pos >= 0; pos--)
	{
		wndDescr = (LPWNDDESCR) app->m_wndArray.GetAt(pos);
		if(wndDescr->m_hWnd == hWnd)
		{
			if(CTime::GetCurrentTime() >= (wndDescr->m_createTime + CTimeSpan( 0, 0, 0, 10)) )
			{
				isFound = wndDescr->m_wndId;
				break;
			}
		}
	}
	csl.Unlock();
	if(isFound > -1)
	{
		ShowWindowByWndId(isFound);
	}
	return TRUE;
}


void CMainFrame::HandleAppMenu(HWND hWnd, POINT point, int typeHidden)
{
	CEZMenu ezmenu;
	CMenu menu;
	CHideApp* app = (CHideApp*) AfxGetApp();
	int menuItemCount = 0;
	LPWNDDESCR wndDescr;
	CString menuItemText;
	BOOL isAutoMenu = m_bAutoMenuEnable;
	BOOL isMenuDialog = m_isMenuDialogActive;
	BOOL isPwdUsed = typeHidden?app->m_isPwdUseShowMenu:app->m_isPwdUseShowPrivateMenu;
	int appItemCount = 0;

	if(m_isMenuDialogActive)
		return;

	if(isPwdUsed && !CheckPassword(app->m_wHash))
	{
		return;
	}
	//
	m_isMenuDialogActive = TRUE;
	m_bAutoMenuEnable = FALSE;
	menuItemCount+= app->m_isAddHideAllApp?1:0;
	menuItemCount+= app->m_isAddShowAllApp?1:0;

	switch(typeHidden)
	{
		case 0:	appItemCount = app->m_globalInVisibleWndCount;
				break;
		case 1:	appItemCount = app->m_globalVisibleWndCount;
				break;
		case 2:	appItemCount = app->m_globalVisibleInVisibleWndCount;
				break;
	}
	menuItemCount+= appItemCount;
	BOOL isAddShowAllCommand = ((appItemCount > 0)&&(app->m_isAddShowAllApp));
	menuItemCount-= isAddShowAllCommand?0:1;
	menu.LoadMenu(IDM_APP_MENU);
	ezmenu.Attach(menu.GetSubMenu(0)->GetSafeHmenu());
	CBitmap bmp;
//	bmp.LoadBitmap(IDB_MENU_TITLE/*The title bitmap*/);
	ezmenu.SetTotalItems(menuItemCount);
	ezmenu.SetMenuBitmap((HBITMAP)bmp.Detach());
	ezmenu.SetBitmapDimension(CSize(6,0 /*20,94*/));
	ezmenu.SetLargeIcon(!app->m_isSmallIcons);
	ezmenu.SetExtensionColor(::GetSysColor(COLOR_ACTIVECAPTION)/*RGB(255,127,170)*/);
	ezmenu.RemoveMenu(IDM_SHOWALL,MF_BYCOMMAND);
	ezmenu.RemoveMenu(IDM_HIDEALL,MF_BYCOMMAND);
	//
	CSingleLock csl(&app->m_CSList);
	csl.Lock();
	appItemCount = 0;
	for(int pos = app->m_wndArray.GetUpperBound(); pos >= 0; pos--)
	{
		wndDescr = (LPWNDDESCR) app->m_wndArray.GetAt(pos);
		if((wndDescr->m_typeHidden == typeHidden) && (wndDescr->m_isMaster == 1))
		{
			LPEZMENUITEMDATA lpez=new CEZMenuItemData;
			ezmenu.m_pArrData.Add( lpez );
			
			lpez->hItemIconLarge = wndDescr->m_bigIcon;
			lpez->hItemIconSmall = wndDescr->m_smallIcon;
			lpez->strItemText= wndDescr->m_windowText;
			ezmenu.AppendOwnerDrawnMenuItem(wndDescr->m_wndId,lpez);
			appItemCount++;
		}
	}
	csl.Unlock();
	//	
	// Add menu separator, if needed
	app->m_typeShowAll = typeHidden;
	//
	if(typeHidden == 1)
	{
		if( ((app->m_isAddHideAllApp) || (app->m_isAddHideAllApp)) && (appItemCount > 0) )
		{
			LPEZMENUITEMDATA lpez=new CEZMenuItemData;
			ezmenu.m_pArrData.Add( lpez );
			lpez->bSeperator=TRUE;
			lpez->strItemText= "";
			ezmenu.AppendOwnerDrawnMenuItem(IDM_SHOWALL+10,lpez);
		}
		// Add Show All command
		if( app->m_isAddShowAllApp && isAddShowAllCommand )
		{
			LPEZMENUITEMDATA lpez=new CEZMenuItemData;
			ezmenu.m_pArrData.Add( lpez );
			lpez->hItemIconLarge = (HICON)LoadImage(app->m_hInstance, MAKEINTRESOURCE(IDI_SHOWALL), IMAGE_ICON, 32, 32, 0);
			lpez->hItemIconSmall = (HICON)LoadImage(app->m_hInstance, MAKEINTRESOURCE(IDI_SHOWALL_SMALL), IMAGE_ICON, 16, 16, 0);
			menuItemText.LoadString(IDS_APPMENUSHOWALL);
			lpez->strItemText = menuItemText;
			ezmenu.AppendOwnerDrawnMenuItem(IDM_SHOWALL,lpez);
		}
		// Add Hide All command
		if( app->m_isAddHideAllApp )
		{
			LPEZMENUITEMDATA lpez=new CEZMenuItemData;
			ezmenu.m_pArrData.Add( lpez );
			lpez->hItemIconLarge = (HICON)LoadImage(app->m_hInstance, MAKEINTRESOURCE(IDI_HIDEALL), IMAGE_ICON, 32, 32, 0);
			lpez->hItemIconSmall = (HICON)LoadImage(app->m_hInstance, MAKEINTRESOURCE(IDI_HIDEALL_SMALL), IMAGE_ICON, 16, 16, 0);
			menuItemText.LoadString(IDS_APPMENUHIDEALL);
			lpez->strItemText = menuItemText;
			ezmenu.AppendOwnerDrawnMenuItem(IDM_HIDEALL,lpez);
		}
	}
	if( (appItemCount > 0) || (typeHidden == 1))
	{
		SetForegroundWindow();
		ezmenu.SetDrawItemCallBack(EZDrawItemCallBack);
		ezmenu.TrackPopupMenu(/*TPM_RECURSE|*/TPM_LEFTALIGN,point.x,point.y,this);
		PostMessage(WM_NULL, 0, 0);
	}
	else
		::MessageBox(0,"No hidden windows!","Hide It!",0);
	m_bAutoMenuEnable = isAutoMenu;
	m_isMenuDialogActive = isMenuDialog;
}

void CMainFrame::HideAllWindows()
{
	m_isHidingActive = TRUE;
	m_isHideAllAppActive = TRUE;
	CEnumWnd enumWnd;
	CHideApp* app = (CHideApp*) AfxGetApp();

	if(enumWnd.EnumWnd())
	{
		for (int i=0;i < enumWnd.m_wndInfoArray.GetSize();i++)
		{
			LPWNDINFO wndInfo = (LPWNDINFO) enumWnd.m_wndInfoArray.ElementAt(i);
			HideWindow(wndInfo->hWnd, eventHIDEALL);
		}
	}
	m_isHidingActive = FALSE;
	m_isHideAllAppActive = FALSE;
}

void CMainFrame::OnShowall()
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	LPWNDDESCR wndDescr;
	int	k = 0;
	int arr[2000];
	for(int i = app->m_wndArray.GetUpperBound(); i >= 0; i--)
	{
		wndDescr = (LPWNDDESCR) app->m_wndArray.GetAt(i);
		switch(app->m_typeShowAll)
		{
			case	-1:
				arr[k] = wndDescr->m_wndId;
				k++;
				break;
			case	1:
				if(wndDescr->m_typeHidden == 1)
				arr[k] = wndDescr->m_wndId;
				k++;
				break;
			default:
				break;
		}
	}
	for(i = 0; i< k; i++)
	{
		ShowWindowByWndId(arr[i]);
	}
}

void CMainFrame::ShowLastHiddenApplication()
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	LPWNDDESCR wndDescr;
	CSingleLock csl(&app->m_CSList);

	csl.Lock();
	if(app->m_wndArray.GetSize() > 0)
	{
		wndDescr = (LPWNDDESCR) app->m_wndArray.GetAt(app->m_wndArray.GetUpperBound());
		if(wndDescr->m_typeHidden == 1)
			ShowWindowByWndId(wndDescr->m_wndId);
	}
	csl.Unlock();
}

void CMainFrame::OnHideall()
{
	HideAllWindows();
}

void CMainFrame::OnSettings()
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	CMyPropertySheet PropSheet;
	CPasswordDialog passwordDlg;
	
	if(m_isMenuDialogActive)
		return;
	// 
	if(app->m_isPwdUseSettings && !CheckPassword(app->m_wHash))
	{
		return;
	}
	m_isMenuDialogActive = TRUE;

	CPPageCommon pageCommon;
	CPPageAppMenu pageAppMenu;
	CPPageHide pageHide;
	CPPagePerfomance pagePerfomance;
	CPPageHotKeys pageHotKeys;
	CPPagePasswords pagePasswords;
	SetLocalVariables(&pageCommon, &pageAppMenu, &pageHide, &pagePerfomance, &pageHotKeys, &pagePasswords);

	PropSheet.AddPage(&pageCommon);
	PropSheet.AddPage(&pageAppMenu);
	PropSheet.AddPage(&pageHide);
	PropSheet.AddPage(&pagePasswords);
	PropSheet.AddPage(&pagePerfomance);
	PropSheet.AddPage(&pageHotKeys);
	PropSheet.SetTitle(_T("Settings "));
	PropSheet.SetActivePage(m_lastPropSheetActivePage);

	int propResult = PropSheet.DoModal();
	if(propResult == IDOK)
	{
		SetGlobalVariables(&pageCommon, &pageAppMenu, &pageHide, &pagePerfomance, &pageHotKeys, &pagePasswords);
		WriteSettings();
	}
	m_isMenuDialogActive = FALSE;
}

BOOL CMainFrame::ReadSettings()
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	UINT wWinModifiers = 0;

	CSingleLock csl(&app->m_CS);
	//WriteLog("D:/log.txt","ReadSettings - m_CS.Lock()");
	csl.Lock();
	//	Common Page
	app->m_isProcessInvisibleWindows = app->GetProfileInt(_T("Settings"), _T("isProcessInvisibleWindows"), 1);
	app->m_isSplashWindow = app->GetProfileInt(_T("Settings"), _T("isSplashWindow"), 1);
	app->m_isAddHideIt = app->GetProfileInt(_T("Settings"), _T("isAddHideIt"), 0);
	app->m_isAddHideAll = app->GetProfileInt(_T("Settings"), _T("isAddHideAll"), 0);
	app->m_isAutoStart = app->GetProfileInt(_T("Settings"), _T("isAutoStart"), 0);
	app->m_isSoftExit = app->GetProfileInt(_T("Settings"), _T("isSoftExit"), 0);
	//	AppMenu Page
	app->m_isSmallIcons = app->GetProfileInt(_T("Settings"), _T("isSmallIcons"), 0);
	app->m_isTrimCaption = app->GetProfileInt(_T("Settings"), _T("isTrimCaption"), 0);
	app->m_trimCaptionChars = app->GetProfileString(_T("Settings"), _T("trimCaptionChars"), _T("10"));
	app->m_isAddHideAllApp = app->GetProfileInt(_T("Settings"), _T("isAddHideAllApp"), 1);
	app->m_isAddShowAllApp = app->GetProfileInt(_T("Settings"), _T("isAddShowAllApp"), 1);
	app->m_isExtractIconFromFile = app->GetProfileInt(_T("Settings"), _T("isExtractIconFromFile"), 1);
	//	Hide Page
	app->m_isHideWhenMinimize = app->GetProfileInt(_T("Settings"), _T("isHideWhenMinimize"), 0);
	app->m_isHideWhenCreated = app->GetProfileInt(_T("Settings"), _T("isHideWhenCreated"), 0);
	app->m_isHideByRMouse = app->GetProfileInt(_T("Settings"), _T("isHideByRMouse"), 1);
	app->m_isHideByDblRMouse = app->GetProfileInt(_T("Settings"), _T("isHideByDblRMouse"), 1);
	app->m_isShowPopupWndMenu = app->GetProfileInt(_T("Settings"), _T("isShowPopupWndMenu"), 1);
	app->m_hideMethod = app->GetProfileInt(_T("Settings"), _T("hideMethod"), 1);
	app->m_isShowOnActivateApp = app->GetProfileInt(_T("Settings"), _T("isShowOnActivateApp"), 1);
	app->m_hideTo = app->GetProfileInt(_T("Settings"), _T("hideTo"), 0);
	app->m_isActivateNextWindow = app->GetProfileInt(_T("Settings"), _T("isActivateNextWindow"), 1);
	//	Password Page
	app->m_wHash = app->GetProfileInt(_T("Settings"), _T("wHash"), 0);
	app->m_isPwdUseExit = app->GetProfileInt(_T("Settings"), _T("isPwdUseExit"), 0);
	app->m_isPwdUseSettings = app->GetProfileInt(_T("Settings"), _T("isPwdUseSettings"), 0);
	app->m_isPwdUseWndSettings = app->GetProfileInt(_T("Settings"), _T("isPwdUseWndSettings"), 0);
	app->m_isPwdUseShowMenu = app->GetProfileInt(_T("Settings"), _T("isPwdUseShowMenu"), 0);
	app->m_isPwdUseShowPrivateMenu = app->GetProfileInt(_T("Settings"), _T("isPwdUseShowPrivateMenu"), 0);
	app->m_isPwdUseShowWindow = app->GetProfileInt(_T("Settings"), _T("isPwdUseShowWindow"), 0);
	//	Perfomance Page (some values only for NT based system)
	app->m_isChangeProcessPriority = app->GetProfileInt(_T("Settings"), _T("isChangeProcessPriority"), 0);
	app->m_processPriority = app->GetProfileInt(_T("Settings"), _T("processPriority"), 1);
	app->m_isDontChangeProcessPriorityPM = app->GetProfileInt(_T("Settings"), _T("isDontChangeProcessPriorityPM"), 1);
#ifndef _DEBUG
	if(app->m_osi.IsNT())
#endif
	{
		app->m_isReduceProcessMemory = app->GetProfileInt(_T("Settings"), _T("isReduceProcessMemory"), 0);
		app->m_isReduceAllProcessesMemory = app->GetProfileInt(_T("Settings"), _T("isReduceAllProcessesMemory"), 0);
		app->m_delayOptimHiddenApp_index = app->GetProfileInt(_T("Settings"), _T("delayOptimHiddenApp_index"), 3);
		app->m_delayOptimHiddenApp = app->GetProfileInt(_T("Settings"), _T("delayOptimHiddenApp"), 10000);
		app->m_delayOptimAllApp_index = app->GetProfileInt(_T("Settings"), _T("delayOptimAllApp_index"), 3);
		app->m_delayOptimAllApp = app->GetProfileInt(_T("Settings"), _T("delayOptimAllApp"), 10000);
	}
	//	HotKey Page
	CString vName;
	for(int i = 0; i < 10; i ++)
	{
		vName = app->m_hotKeyActionNames[i] + _T("_wVirtualKeyCode");
		app->m_hotKeyArray[i][0] = app->GetProfileInt(_T("Settings"), vName, 0);
		vName = app->m_hotKeyActionNames[i] + _T("_wModifiers");
		app->m_hotKeyArray[i][1] = app->GetProfileInt(_T("Settings"), vName, 0);
		//
		if(app->m_hotKeyArray[i][0] != 0)
		{
			wWinModifiers = 0;
			if( (app->m_hotKeyArray[i][1] & HOTKEYF_ALT) > 0)
				wWinModifiers = wWinModifiers | MOD_ALT;
			if( (app->m_hotKeyArray[i][1] & HOTKEYF_CONTROL) > 0)
				wWinModifiers = wWinModifiers | MOD_CONTROL;
			if( (app->m_hotKeyArray[i][1] & HOTKEYF_SHIFT) > 0)
				wWinModifiers = wWinModifiers | MOD_SHIFT;
			//
			UnregisterHotKey(this->GetSafeHwnd(), app->m_hotKeyArray[i][2]);
			if(!RegisterHotKey(this->GetSafeHwnd(), app->m_hotKeyArray[i][2], wWinModifiers, app->m_hotKeyArray[i][0]))
			{
				UnregisterHotKey(this->GetSafeHwnd(), app->m_hotKeyArray[i][2]);
				app->m_hotKeyArray[i][0] = 0;
				app->m_hotKeyArray[i][1] = 0;
			}
		}
	}
	//WriteLog("D:/log.txt","ReadSettings - m_CS.Unlock()");
	csl.Unlock();
	return TRUE;
}

BOOL CMainFrame::WriteSettings()
{
	CHideApp* app = (CHideApp*) AfxGetApp();

	CSingleLock csl(&app->m_CS);
	//WriteLog("D:/log.txt","OnSettings - m_CS.Lock()");
	csl.Lock();
	//	Common Page
	app->WriteProfileInt(_T("Settings"), _T("isHideWhenMinimize"), app->m_isHideWhenMinimize);
	app->WriteProfileInt(_T("Settings"), _T("isProcessInvisibleWindows"), app->m_isProcessInvisibleWindows);
	app->WriteProfileInt(_T("Settings"), _T("isSplashWindow"), app->m_isSplashWindow);
	app->WriteProfileInt(_T("Settings"), _T("isAddHideIt"), app->m_isAddHideIt);
	app->WriteProfileInt(_T("Settings"), _T("isAddHideAll"), app->m_isAddHideAll);
	app->WriteProfileInt(_T("Settings"), _T("isAutoStart"), app->m_isAutoStart);
	app->WriteProfileInt(_T("Settings"), _T("isSoftExit"), app->m_isSoftExit);
	//	Hide Page
	app->WriteProfileInt(_T("Settings"), _T("isHideWhenMinimize"), app->m_isHideWhenMinimize);
	app->WriteProfileInt(_T("Settings"), _T("isHideWhenCreated"), app->m_isHideWhenCreated);
	app->WriteProfileInt(_T("Settings"), _T("isHideByRMouse"), app->m_isHideByRMouse);
	app->WriteProfileInt(_T("Settings"), _T("isHideByDblRMouse"), app->m_isHideByDblRMouse);
	app->WriteProfileInt(_T("Settings"), _T("isShowPopupWndMenu"), app->m_isShowPopupWndMenu);
	app->WriteProfileInt(_T("Settings"), _T("hideMethod"), app->m_hideMethod);
	app->WriteProfileInt(_T("Settings"), _T("isShowOnActivateApp"), app->m_isShowOnActivateApp);
	app->WriteProfileInt(_T("Settings"), _T("hideTo"), app->m_hideTo);
	app->WriteProfileInt(_T("Settings"), _T("isActivateNextWindow"), app->m_isActivateNextWindow);
	//	Password Page
	app->WriteProfileInt(_T("Settings"), _T("wHash"), app->m_wHash);
	app->WriteProfileInt(_T("Settings"), _T("isPwdUseExit"), app->m_isPwdUseExit);
	app->WriteProfileInt(_T("Settings"), _T("isPwdUseSettings"), app->m_isPwdUseSettings);
	app->WriteProfileInt(_T("Settings"), _T("isPwdUseWndSettings"), app->m_isPwdUseWndSettings);
	app->WriteProfileInt(_T("Settings"), _T("isPwdUseShowMenu"), app->m_isPwdUseShowMenu);
	app->WriteProfileInt(_T("Settings"), _T("isPwdUseShowPrivateMenu"), app->m_isPwdUseShowPrivateMenu);
	app->WriteProfileInt(_T("Settings"), _T("isPwdUseShowWindow"), app->m_isPwdUseShowWindow);
	//	AppMenu Page
	app->WriteProfileInt(_T("Settings"), _T("isSmallIcons"), app->m_isSmallIcons);
	app->WriteProfileInt(_T("Settings"), _T("isTrimCaption"), app->m_isTrimCaption);
	app->WriteProfileString(_T("Settings"), _T("trimCaptionChars"), app->m_trimCaptionChars);
	app->WriteProfileInt(_T("Settings"), _T("isAddHideAllApp"), app->m_isAddHideAllApp);
	app->WriteProfileInt(_T("Settings"), _T("isAddShowAllApp"), app->m_isAddShowAllApp);
	app->WriteProfileInt(_T("Settings"), _T("isExtractIconFromFile"), app->m_isExtractIconFromFile);
	//	Perfomance Page (some values only for NT based system)
	app->WriteProfileInt(_T("Settings"), _T("isChangeProcessPriority"), app->m_isChangeProcessPriority);
	app->WriteProfileInt(_T("Settings"), _T("processPriority"), app->m_processPriority);
	app->WriteProfileInt(_T("Settings"), _T("isDontChangeProcessPriorityPM"), app->m_isDontChangeProcessPriorityPM);
#ifndef _DEBUG
	if(app->m_osi.IsNT())
#endif
	{
		app->WriteProfileInt(_T("Settings"), _T("isReduceProcessMemory"), app->m_isReduceProcessMemory);
		app->WriteProfileInt(_T("Settings"), _T("isReduceAllProcessesMemory"), app->m_isReduceAllProcessesMemory);
		app->WriteProfileInt(_T("Settings"), _T("delayOptimHiddenApp_index"), app->m_delayOptimHiddenApp_index);
		app->WriteProfileInt(_T("Settings"), _T("delayOptimHiddenApp"), app->m_delayOptimHiddenApp);
		app->WriteProfileInt(_T("Settings"), _T("delayOptimAllApp_index"), app->m_delayOptimAllApp_index);
		app->WriteProfileInt(_T("Settings"), _T("delayOptimAllApp"), app->m_delayOptimAllApp);
	}
	//	HotKey Page
	CString vName;
	for(int i = 0; i < 10; i ++)
	{
		vName = app->m_hotKeyActionNames[i] + _T("_wVirtualKeyCode");
		app->WriteProfileInt(_T("Settings"), vName, app->m_hotKeyArray[i][0]);
		vName = app->m_hotKeyActionNames[i] + _T("_wModifiers");
		app->WriteProfileInt(_T("Settings"), vName, app->m_hotKeyArray[i][1]);
	}
	//
	CString appName = app->m_pszHelpFilePath;
	appName = appName.Left(appName.GetLength()-3) + _T("EXE");
	if(app->m_isAutoStart)
	{
		CRegKey regKey;
		if(regKey.Create(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run")==ERROR_SUCCESS)
			regKey.SetValue(appName, AfxGetAppName());
	}
	else
	{
		CRegKey regKey;
		if(regKey.Create(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run")==ERROR_SUCCESS)
			regKey.DeleteValue(AfxGetAppName());
	}
	csl.Unlock();
	return TRUE;
}


BOOL CMainFrame::OnQueryEndSession() 
{
	//
	if (!CFrameWnd::OnQueryEndSession())
		return FALSE;
	CHideApp* app = (CHideApp*) AfxGetApp();
	app->ExitInstanceProcess();
	app->m_isPwdUseExit = FALSE;
	app->m_isPwdUseShowWindow = FALSE;
	app->m_isProgramQuit = TRUE;
	OnShowall();
	app->ModifyAllWindows(FALSE);
	PostMessage(WM_COMMAND, (WPARAM) ID_APP_EXIT, 0);
	return TRUE;
}

void CMainFrame::OnRButtonDown(UINT nFlags, CPoint point) 
{
	HandleContextMenu(m_hWnd, point);
	//
	CFrameWnd::OnRButtonDown(nFlags, point);
}

void CMainFrame::HandleContextMenu(HWND hWnd, POINT point)
{
	return;
}

void CMainFrame::SetGlobalVariables(CPPageCommon* pageCommon, CPPageAppMenu* pageAppMenu, CPPageHide* pageHide, CPPagePerfomance* pagePerfomance, CPPageHotKeys* pageHotKeys, CPPagePasswords* pagePasswords)
{

	CHideApp* app = (CHideApp*) AfxGetApp();
	CSingleLock csl(&app->m_CS);
	//WriteLog("D:/log.txt","OnSettings - m_CS.Lock()");
	csl.Lock();
	//	Common Page
	app->m_isProcessInvisibleWindows = pageCommon->m_isProcessInvisibleWindows;
	app->m_isSplashWindow = pageCommon->m_isSplashWindow;
	app->m_isAddHideIt = pageCommon->m_isAddHideIt;
	app->m_isAddHideAll = pageCommon->m_isAddHideAll;
	app->m_isAutoStart = pageCommon->m_isAutoStart;
	app->m_isSoftExit = pageCommon->m_isSoftExit;
	//	AppMenu Page
	app->m_isSmallIcons = pageAppMenu->m_isSmallIcons;
	app->m_isTrimCaption = pageAppMenu->m_isTrimCaption;
	app->m_trimCaptionChars = pageAppMenu->m_trimCaptionChars;
	app->m_isAddHideAllApp = pageAppMenu->m_isAddHideAllApp;
	app->m_isAddShowAllApp = pageAppMenu->m_isAddShowAllApp;
	app->m_isExtractIconFromFile = pageAppMenu->m_isExtractIconFromFile;
	//	Hide Page
	app->m_isHideWhenMinimize = pageHide->m_isHideWhenMinimize;
	app->m_isHideByRMouse = pageHide->m_isHideByRMouse;
	app->m_isHideByDblRMouse = pageHide->m_isHideByDblRMouse;
	app->m_isShowPopupWndMenu = pageHide->m_isShowPopupWndMenu;
	app->m_hideMethod = pageHide->m_hideMethod;
	app->m_isShowOnActivateApp = pageHide->m_isShowOnActivateApp;
	app->m_hideTo = pageHide->m_hideTo;
	app->m_isActivateNextWindow = pageHide->m_isActivateNextWindow;
	//	Password Page
	if(	(pagePasswords->m_masterPassword != pagePasswords->m_oldMasterPassword) &&
		(pagePasswords->m_masterPassword == pagePasswords->m_confirmMasterPassword))
		app->m_wHash = GetPasswordHash(pagePasswords->m_masterPassword);
	app->m_isPwdUseExit = pagePasswords->m_isPwdUseExit;
	app->m_isPwdUseSettings = pagePasswords->m_isPwdUseSettings;
	app->m_isPwdUseWndSettings = pagePasswords->m_isPwdUseWndSettings;
	app->m_isPwdUseShowMenu = pagePasswords->m_isPwdUseShowMenu;
	app->m_isPwdUseShowPrivateMenu = pagePasswords->m_isPwdUseShowPrivateMenu;
	app->m_isPwdUseShowWindow = pagePasswords->m_isPwdUseShowWindow;
	//	Perfomance Page (some values only for NT based system)
	app->m_isChangeProcessPriority = pagePerfomance->m_isChangeProcessPriority;
	app->m_processPriority = pagePerfomance->m_processPriority;
	app->m_isDontChangeProcessPriorityPM = pagePerfomance->m_isDontChangeProcessPriorityPM;
	app->m_isReduceProcessMemory = pagePerfomance->m_isReduceProcessMemory;
	app->m_isReduceAllProcessesMemory = pagePerfomance->m_isReduceAllProcessesMemory;
	app->m_delayOptimHiddenApp_index = pagePerfomance->m_delayOptimHiddenApp_index;
	app->m_delayOptimHiddenApp = pagePerfomance->m_delayOptimHiddenApp;
	app->m_delayOptimAllApp_index = pagePerfomance->m_delayOptimAllApp_index;
	app->m_delayOptimAllApp = pagePerfomance->m_delayOptimAllApp;
	//	HotKey Page
	for(int i = 0; i < 10; i ++)
	{
		app->m_hotKeyArray[i][0] = pageHotKeys->m_hotKeyArray[i][0];
		app->m_hotKeyArray[i][1] = pageHotKeys->m_hotKeyArray[i][1];
		app->m_hotKeyArray[i][2] = pageHotKeys->m_hotKeyArray[i][2];
	}
	csl.Unlock();
}

void CMainFrame::SetLocalVariables(CPPageCommon* pageCommon, CPPageAppMenu* pageAppMenu, CPPageHide* pageHide, CPPagePerfomance* pagePerfomance, CPPageHotKeys* pageHotKeys, CPPagePasswords* pagePasswords)
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	CSingleLock csl(&app->m_CS);
	csl.Lock();
	//	Common Page
	pageCommon->m_isProcessInvisibleWindows = app->m_isProcessInvisibleWindows;
	pageCommon->m_isSplashWindow = app->m_isSplashWindow;
	pageCommon->m_isAddHideIt = app->m_isAddHideIt;
	pageCommon->m_isAddHideAll = app->m_isAddHideAll;
	pageCommon->m_isAutoStart = app->m_isAutoStart;
	pageCommon->m_isSoftExit = app->m_isSoftExit;
	//	AppMenu Page
	pageAppMenu->m_isSmallIcons = app->m_isSmallIcons;
	pageAppMenu->m_isTrimCaption = app->m_isTrimCaption;
	pageAppMenu->m_trimCaptionChars = app->m_trimCaptionChars;
	pageAppMenu->m_isAddHideAllApp = app->m_isAddHideAllApp;
	pageAppMenu->m_isAddShowAllApp = app->m_isAddShowAllApp;
	pageAppMenu->m_isExtractIconFromFile = app->m_isExtractIconFromFile;
	//	Hide Page
	pageHide->m_isHideWhenMinimize = app->m_isHideWhenMinimize;
	pageHide->m_isHideByRMouse = app->m_isHideByRMouse;
	pageHide->m_isHideByDblRMouse = app->m_isHideByDblRMouse;
	pageHide->m_isShowPopupWndMenu = app->m_isShowPopupWndMenu;
	pageHide->m_hideMethod = app->m_hideMethod;
	pageHide->m_isShowOnActivateApp = app->m_isShowOnActivateApp;
	pageHide->m_hideTo = app->m_hideTo;
	pageHide->m_isActivateNextWindow = app->m_isActivateNextWindow;
	//	Password Page
	//pagePasswords->m_masterPassword = app->m_wHash;
	if(app->m_wHash == 0)
	{
		pagePasswords->m_masterPassword = _T("");
		pagePasswords->m_confirmMasterPassword = _T("");
	}
	pagePasswords->m_isPwdUseExit = app->m_isPwdUseExit;
	pagePasswords->m_isPwdUseSettings = app->m_isPwdUseSettings;
	pagePasswords->m_isPwdUseWndSettings = app->m_isPwdUseWndSettings;
	pagePasswords->m_isPwdUseShowMenu = app->m_isPwdUseShowMenu;
	pagePasswords->m_isPwdUseShowPrivateMenu = app->m_isPwdUseShowPrivateMenu;
	pagePasswords->m_isPwdUseShowWindow = app->m_isPwdUseShowWindow;
	//	Perfomance Page (some values only for NT based system)
	pagePerfomance->m_isChangeProcessPriority = app->m_isChangeProcessPriority;
	pagePerfomance->m_processPriority = app->m_processPriority;
	pagePerfomance->m_isDontChangeProcessPriorityPM = app->m_isDontChangeProcessPriorityPM;
	pagePerfomance->m_isReduceProcessMemory = app->m_isReduceProcessMemory;
	pagePerfomance->m_isReduceAllProcessesMemory = app->m_isReduceAllProcessesMemory;
	pagePerfomance->m_delayOptimHiddenApp_index = app->m_delayOptimHiddenApp_index;
	pagePerfomance->m_delayOptimHiddenApp = app->m_delayOptimHiddenApp;
	pagePerfomance->m_delayOptimAllApp_index = app->m_delayOptimAllApp_index;
	pagePerfomance->m_delayOptimAllApp = app->m_delayOptimAllApp;
	//	HotKey Page
	for(int i = 0; i < 10; i ++)
	{
		pageHotKeys->m_hotKeyArray[i][0] = app->m_hotKeyArray[i][0];
		pageHotKeys->m_hotKeyArray[i][1] = app->m_hotKeyArray[i][1];
		pageHotKeys->m_hotKeyArray[i][2] = app->m_hotKeyArray[i][2];
	}
	//
	csl.Unlock();
}


void CMainFrame::OnWndsettingspopup() 
{
	OnWndsettings();
}

void CMainFrame::OnSettingspopup() 
{
	OnSettings();
}
