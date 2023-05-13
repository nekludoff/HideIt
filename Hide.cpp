// Hide.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Hide.h"
#include "AboutBox.h"
#include "MainFrm.h"
#include "WndDescr.h"
#include "./WHook/WHook.h"
#include "EnumWnd.h"
#include "Afxmt.h"
#include "debug.h"
#include "TrialDialog.h"
#include "asprotect.h"
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

volatile BOOL g_OptimizeAllThreadRun = FALSE;
volatile BOOL g_OptimizeAllThreadStarting = FALSE;
volatile BOOL g_OptimizeHiddenThreadRun = FALSE;
volatile BOOL g_OptimizeHiddenThreadStarting = FALSE;
volatile BOOL g_CheckWndInListThreadRun = FALSE;
volatile BOOL g_CheckWndInListThreadStarting = FALSE;
volatile BOOL g_CheckAutoHideWndThreadRun = FALSE;
volatile BOOL g_CheckAutoHideWndThreadStarting = FALSE;
volatile BOOL g_ExitInstance = FALSE;
volatile UINT g_delayOptimAllApp;
volatile UINT g_delayOptimHideApp;
volatile UINT g_delayCheckWndInList;
volatile UINT g_delayCheckAutoHideWnd;
void GSleep(LPCTSTR lpName, UINT uMillisec);
UINT OptimizeAllThreadProc(LPVOID pParam);
UINT OptimizeHideThreadProc(LPVOID pParam);
UINT CheckWndInListThreadProc(LPVOID pParam);
UINT CheckAutoHideWndThreadProc(LPVOID pParam);
BOOL isTrial = FALSE;

char*	UserName			= NULL;
char*	LocalHardwareID		= NULL;

int		TrialDaysTotal		= -1,
		TrialDaysLeft		= -1;

#ifndef _DEBUG
#define RELEASE
#endif

typedef void (*TProcedure) ();

//----------------------------------------------------------------------
// Try to get a registration information.
// ASProtect will set this information after a checking of a registraion 
// key
//----------------------------------------------------------------------
void __declspec(dllexport) __stdcall GetRegistrationInformation(char* NewUserName)
{
	UserName = _T("Free version. Reassembled 2023");//NewUserName;
}
//----------------------------------------------------------------------
// ASProtect will set a trial information after a checking of a registraion 
// key
//----------------------------------------------------------------------
void __declspec(dllexport) __stdcall GetTrialDays(int Total, int Left)
{
	TrialDaysTotal = Total;
	TrialDaysLeft  = Left;
}

//#ifdef RELEASE
#ifndef _DEBUG
void __declspec(dllexport) ExecuteApplication();
TProcedure	ASProtectRunApplication = ExecuteApplication;

//void __declspec(dllexport) ExecuteTrial();
//TProcedure	ASProtectRunApplication = ExecuteTrial;
#else
void __declspec(dllexport) ExecuteApplication();
TProcedure	ASProtectRunApplication = ExecuteApplication;
#endif
//---------------------------------------------------------------------
//  This procedure will be called by ASProtect if:
//  1. Evaluation period of application has expired
//---------------------------------------------------------------------
void __declspec(dllexport) ExecuteTrial()
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	CTrialDialog dlgTrial;
	dlgTrial.DoModal();
	isTrial = TRUE;
}
//------------------------------------------------
//  This procedure will be called by ASProtect if:
//  1. Trial doesn't expire
//  2. Application was registered
//------------------------------------------------
void __declspec(dllexport) ExecuteApplication()
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	isTrial = FALSE;
}
//---------------------------------------------------------------------
//  This procedure will be called by ASProtect
//  and ASProtect will pass the pointer to dispatcher
//---------------------------------------------------------------------
void __declspec(dllexport) __stdcall GetRunApplicationFunction(TProcedure Ptr){

  ASProtectRunApplication = Ptr;
}

/////////////////////////////////////////////////////////////////////////////
// CHideApp

BEGIN_MESSAGE_MAP(CHideApp, CWinApp)
	//{{AFX_MSG_MAP(CHideApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHideApp construction

CHideApp::CHideApp()
{
	m_hMutex = NULL;
	m_IDM_MYMENU  = 58416 & 0xFFF0;
	m_IDM_MYMENUALL  = 58432 & 0xFFF0;
	m_countHWND = 0;
	// saved variables
	m_isAddHideAll = FALSE;
	m_isAddHideIt = FALSE;
	m_isSplashWindow = FALSE;
	m_isProcessInvisibleWindows = FALSE;
	m_isHideWhenMinimize = FALSE;
	m_isSmallIcons = FALSE;
	m_isTrimCaption = FALSE;
	m_trimCaptionChars = _T("");
	m_isAddHideAllApp = FALSE;
	m_isAddShowAllApp = FALSE;
	m_delayOptimAllApp = 10;
	m_delayOptimAllApp_index = 0;
	m_delayOptimHiddenApp_index = 0;
	m_delayOptimHiddenApp = 10;
	m_isReduceProcessMemory  = FALSE;
	m_isReduceAllProcessesMemory = FALSE;
	m_isExtractIconFromFile = FALSE;
	m_isSoftExit = FALSE;
	m_isHideByRMouse = FALSE;
	m_isHideWhenCreated = FALSE;
	m_isHideByDblRMouse = FALSE;
	m_isShowOnActivateApp = FALSE;
	m_hideTo = 0;
	m_isChangeProcessPriority = FALSE;
	m_processPriority = 1;
	m_hideMethod = 1;
	m_isShowPopupWndMenu = TRUE;
	m_isActivateNextWindow = TRUE;
	m_isDontChangeProcessPriorityPM = TRUE;
	m_PMProcessId = 0;
	m_PMThreadId = 0;
	m_infoStoreFileName = __argv[0];
	m_wndInfoStoreFileName = __argv[0];
	m_infoStoreFileName = m_infoStoreFileName.Left(m_infoStoreFileName.GetLength()-3) + _T("LOG");
	m_wndInfoStoreFileName = m_wndInfoStoreFileName.Left(m_wndInfoStoreFileName.GetLength()-3) + _T("WND");
//	m_wndInfoStoreImageList = new CImageList();
//	ASSERT(m_pWndInfoStoreImageList != NULL);    // serious allocation failure checking
	m_wndInfoStoreImageList.Create(16, 16, ILC_COLOR4|ILC_MASK, 8, 4);
	m_globalVisibleWndCount = 0;
	m_globalInVisibleWndCount = 0;
	m_globalVisibleInVisibleWndCount = 0;
	m_wHash = 0;
	m_isPwdUseExit = FALSE;
	m_isPwdUseSettings = FALSE;
	m_isPwdUseWndSettings = FALSE;
	m_isPwdUseShowMenu = FALSE;
	m_isPwdUseShowPrivateMenu = FALSE;
	m_isPwdUseShowWindow = FALSE;
	m_typeShowAll = 1;
	m_isProgramQuit = FALSE;
	//
	for(int i = 0; i < 10; i++)
	{
		m_hotKeyActionNames[i] = _T("");
	}
	m_hotKeyActionNames[0] = _T("Window: Hide all windows");
	m_hotKeyActionNames[1] = _T("Window: Show all windows");
	m_hotKeyActionNames[2] = _T("Window: Hide active window");
	m_hotKeyActionNames[3] = _T("Window: Show last hidden window");
	m_hotKeyActionNames[4] = _T("Window: Customize window settings");
	m_hotKeyActionNames[5] = _T("Menu: Show system menu");
	m_hotKeyActionNames[6] = _T("Menu: Show Hide It! program settings");
	m_hotKeyActionNames[7] = _T("Menu: Show common list of windows (window menu)");
	m_hotKeyActionNames[8] = _T("Menu: Show secured private list of windows");
	m_hotKeyActionNames[9] = _T("Menu: Show private list of windows");
	//
	for(i = 0; i < 10; i++)
	{
		CString str = "HIDE_IT_4_0_" + m_hotKeyActionNames[i] + "_ATOM";
		m_hotKeyArray[i][0] = 0;
		m_hotKeyArray[i][1] = 0;
		m_hotKeyArray[i][2] = GlobalAddAtom(str);
	}
	//
	m_trialDaysLeft = -1;
	m_trialDaysTotal = -1;
	m_userName = _T("");
	m_isTrial = FALSE;
	//
	UINT WM_APPPLUS = WM_APP + 101;
	for(i = 0; i < 150; i++)
	{
		m_wmCommandMSG[i][0] = WM_APPPLUS + i;
		m_wmCommandMSG[i][1] = 0;
	}
	//
	p_OptimizeHiddenThread = NULL;
	p_OptimizeAllThread = NULL;
	p_CheckWndInListThread = NULL;
	p_CheckAutoHideWndThread = NULL;
}

CHideApp::~CHideApp()
{
	InstallHooks(FALSE);
//	FreeLibrary((HMODULE)m_dllInstance);
	int i;

	if(m_hMutex)
		::CloseHandle(m_hMutex);

	for(i = 0; i < m_wndArray.GetSize(); i++)
	{
		LPWNDDESCR wndDescr = (LPWNDDESCR) m_wndArray.GetAt(0);
		m_wndArray.RemoveAt(0);
		delete wndDescr;
	}
	m_wndArray.RemoveAll();

	int k = m_wndInfoStoreArray.GetSize();
	for(i = 0; i < k; i++)
	{
		CWndInfoStore* wndInfo = (CWndInfoStore*) m_wndInfoStoreArray.GetAt(0);
		m_wndInfoStoreArray.RemoveAt(0);
		delete wndInfo;
	}
	m_wndInfoStoreArray.RemoveAll();

	if(m_pSplashWindow != NULL)
	{
		delete m_pSplashWindow;
	}
	ModifyAllWindows(FALSE);
}

BOOL CHideApp::InstallWHook()
{
	INITPARMS initParms;
	initParms.hMainWindow = m_pMainWnd->GetSafeHwnd();
	initParms.IDM_MYMENU = m_IDM_MYMENU;
	initParms.IDM_MYMENUALL = m_IDM_MYMENUALL;
	initParms.wmSetMenu = m_wmSetMenuMSG;
	initParms.wmRemoveMenu = m_wmRemoveMenuMSG;
	initParms.isAddHideIt = m_isAddHideIt;
	initParms.isAddHideAll = m_isAddHideAll;
	//
	m_dllInstance = InitHooksDll(initParms);
	int i = InstallHooks(FALSE);
	i = InstallHooks(TRUE);
	return (i == 0);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHideApp object

CHideApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHideApp initialization

BOOL CHideApp::InitInstance()
{
	m_hMutex = ::CreateMutex(NULL, FALSE, "HIDE_IT:01:MUTEX:INSTANCE_01_02");
	if( m_hMutex != NULL )
	{   // indicates running instance
		if(::GetLastError()==ERROR_ALREADY_EXISTS)
		{
			return FALSE;   // forbid further processing
		}
	}

	SetRegistryKey(_T("Rokela Software"));

	ASProtectRunApplication();
	if(isTrial)
		return FALSE;

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	m_infoStore.Create(m_infoStoreFileName);
	RestoreOnLog();
	m_wndStore.ReadInfo(m_wndInfoStoreFileName);

	m_wmSetMenuMSG = RegisterWindowMessage("HIDE_SETMENUMSG");
	if(m_wmSetMenuMSG == 0)
		m_wmSetMenuMSG = WM_APP + 90;
	m_wmRemoveMenuMSG = RegisterWindowMessage("HIDE_REMOVEMENUMSG");
	if(m_wmRemoveMenuMSG == 0)
		m_wmRemoveMenuMSG = WM_APP + 91;
	
	m_WM_TASKBARCREATED = RegisterWindowMessage(_T("TaskbarCreated"));

	g_ExitInstance = FALSE;

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	m_nCmdShow = SW_HIDE;	// Hide the main window

	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);


	// The one and only window has been initialized, so show and update it.
	pFrame->ShowWindow(SW_HIDE);
	pFrame->UpdateWindow();

#ifdef _DEBUG
	m_userName = "Debug";
#else
	m_userName = _T("Free version. Reassembled 2023");//UserName;
#endif
	m_trialDaysTotal = 100000;//TrialDaysTotal;
	m_trialDaysLeft = 100000;//TrialDaysLeft;
	
	GetPMThreadProcessId();
	pFrame->SendMessage(WM_APP+30, 0, 0);
	// Show Splash Screen
	if(m_isSplashWindow)
	{
		m_pSplashWindow = new CSplashWindow();
		ASSERT(m_pSplashWindow);
		BOOL ret = m_pSplashWindow->Create(IDD_SPLASHWINDOW, NULL);
		m_pSplashWindow->SetSplashTimerUp(1000);
	}
	//
	if(!InstallWHook())
	{
		AfxMessageBox(_T("Can not install window hooks!"), MB_ICONSTOP);
		return FALSE;
	}
	ModifyAllWindows(TRUE);
	

	return TRUE;
}

void CHideApp::OnAppAbout()
{
	CAboutBox dlgAbout;
	dlgAbout.DoModal();
}

void CHideApp::ModifyAllWindows(BOOL sFlag)
{
	if(sFlag)
		::SendMessage(HWND_BROADCAST, m_wmSetMenuMSG, 0, 0);
	else
		::SendMessage(HWND_BROADCAST, m_wmRemoveMenuMSG, 0, 0);
}

DWORD CHideApp::GetMinWndId()
{
	DWORD minId = 0;
	for(int i = 0; i < 150; i++)
		if(m_wmCommandMSG[i][1] == 0)
		{
			m_wmCommandMSG[i][1] = 1;
			minId = m_wmCommandMSG[i][0];
			break;
		}
	return minId;		
}

void CHideApp::ReleaseWndId(DWORD wndId)
{
	for(int i = 0; i < 150; i++)
		if(m_wmCommandMSG[i][0] == wndId)
		{
			m_wmCommandMSG[i][1] = 0;
			break;
		}
}


BOOL CHideApp::OnIdle(LONG lCount) 
{
	CSingleLock csl(&m_CS);
	m_pMainWnd->ShowWindow(SW_HIDE);
	csl.Lock();
	g_OptimizeHiddenThreadRun = m_isReduceProcessMemory;
	g_delayOptimHideApp = m_delayOptimHiddenApp;
	if(g_OptimizeHiddenThreadRun && !g_OptimizeHiddenThreadStarting)
	{
		g_OptimizeHiddenThreadStarting = TRUE;
		p_OptimizeHiddenThread = AfxBeginThread(OptimizeHideThreadProc, 0, THREAD_PRIORITY_IDLE);
	}
	//
	g_OptimizeAllThreadRun = m_isReduceAllProcessesMemory;
	g_delayOptimAllApp = m_delayOptimAllApp;
	if(g_OptimizeAllThreadRun && !g_OptimizeAllThreadStarting)
	{
		g_OptimizeAllThreadStarting = TRUE;
		p_OptimizeAllThread = AfxBeginThread(OptimizeAllThreadProc, 0, THREAD_PRIORITY_IDLE);
	}
	//
	g_CheckWndInListThreadRun = TRUE;
	g_delayCheckWndInList = 600;
	if(g_CheckWndInListThreadRun && !g_CheckWndInListThreadStarting)
	{
		g_CheckWndInListThreadStarting = TRUE;
		p_CheckWndInListThread = AfxBeginThread(CheckWndInListThreadProc, 0, THREAD_PRIORITY_NORMAL);
	}
	g_CheckAutoHideWndThreadRun = TRUE;
	g_delayCheckAutoHideWnd = 600;
	if(g_CheckAutoHideWndThreadRun && !g_CheckAutoHideWndThreadStarting)
	{
		g_CheckAutoHideWndThreadStarting = TRUE;
		p_CheckAutoHideWndThread = AfxBeginThread(CheckAutoHideWndThreadProc, 0, THREAD_PRIORITY_ABOVE_NORMAL);
	}
	csl.Unlock();
	SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
	return CWinApp::OnIdle(lCount);
}

void CHideApp::OptimizeMemoryUsage()
{
	return;
}

void GSleep(LPCTSTR lpName, UINT uMillisec)
{
	HANDLE h = ::CreateEvent(NULL, FALSE, FALSE, lpName);
	::WaitForSingleObject(h, uMillisec);
	::CloseHandle(h);
}

UINT OptimizeHideThreadProc(LPVOID pParam)
{
	DWORD pid = 0, tid = 0;
	LPWNDDESCR wndDescr;
	HANDLE hProcess = NULL;
	UINT l_delayOptimHideApp = 0;
	CHideApp* app = (CHideApp*) AfxGetApp();
	CSingleLock csl(&app->m_CS);
	CSingleLock csl1(&app->m_CSList);
	while(1)
	{
		csl.Lock();
		csl1.Lock();
		if(!g_OptimizeHiddenThreadRun || g_ExitInstance)
		{
			g_OptimizeHiddenThreadStarting = FALSE;
			csl1.Unlock();
			csl.Unlock();
			break;
		}
		//
		for(int pos = 0; pos < app->m_wndArray.GetSize(); pos++)
		{
			wndDescr = (LPWNDDESCR) app->m_wndArray.GetAt(pos);
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, wndDescr->m_processId);
			if(hProcess != NULL)
			{
				SetProcessWorkingSetSize(hProcess, -1, -1);
			}	
			CloseHandle(hProcess);
			GSleep("OptimizeHideThreadProc_5",4);
		}
		l_delayOptimHideApp = g_delayOptimHideApp;
		csl1.Unlock();
		csl.Unlock();
		//
		//same as: Sleep(g_delayOptimHideApp);
		for(UINT i = 0; (i < (l_delayOptimHideApp/10)); i++)
		{
			csl.Lock();
			l_delayOptimHideApp = g_delayOptimHideApp;
			if(g_ExitInstance)
			{	
				csl.Unlock();
				break;
			}
			csl.Unlock();
			GSleep("OptimizeHideThreadProc_10", 9);
		}
	}
	return 0;
}

UINT OptimizeAllThreadProc(LPVOID pParam)
{
	DWORD pid = 0, tid = 0;
	HANDLE hProcess = NULL;
	UINT l_delayOptimAllApp = 0;
	CEnumWnd* enumWnd;
	CHideApp* app = (CHideApp*) AfxGetApp();
	CSingleLock csl(&app->m_CS);

	while(1)
	{
		csl.Lock();
		if(!g_OptimizeAllThreadRun || g_ExitInstance)
		{
			g_OptimizeAllThreadStarting = FALSE;
			csl.Unlock();
			break;
		}
		l_delayOptimAllApp = g_delayOptimAllApp;
		csl.Unlock();
		enumWnd = new CEnumWnd;
		if(enumWnd->EnumWnd(0))
		{
			for (int i=0;i < enumWnd->m_wndInfoArray.GetSize();i++)
			{
				LPWNDINFO wndInfo = (LPWNDINFO) enumWnd->m_wndInfoArray.ElementAt(i);
				if(wndInfo->processId != 0)
				{			
					hProcess = OpenProcess(PROCESS_SET_QUOTA, FALSE, wndInfo->processId);
					if(hProcess != NULL)
					{
						SetProcessWorkingSetSize(hProcess, -1, -1);
					}
					CloseHandle(hProcess);
				}
				GSleep("OptimizeAllThreadProc_5",4);
			}
		}
		delete enumWnd;
		//same as: Sleep(g_delayOptimAllApp);
		for(UINT i = 0; (i < (l_delayOptimAllApp/10)); i++)
		{
			csl.Lock();
			l_delayOptimAllApp = g_delayOptimAllApp;
			if(g_ExitInstance)
			{	
				csl.Unlock();
				break;
			}
			csl.Unlock();
			GSleep("OptimizeAllThreadProc_10", 9);
		}
	}
	return 0;
}


UINT CheckWndInListThreadProc(LPVOID pParam)
{
	int wndCount;
	HWND aWnds[500];
	HWND aHWnds[500];
	int aWndsCnt;
	int aHWndsCnt;
	CEnumWinModules em;
	LPWNDDESCR wndDescr;
	UINT l_delayCheckWndInList = 0;
	CHideApp* app = (CHideApp*) AfxGetApp();
	CSingleLock csl(&app->m_CS);
	CSingleLock csl1(&app->m_CSList);
	CString progName;

	while(1)
	{
		csl.Lock();
		csl1.Lock();
		if(!g_CheckWndInListThreadRun || g_ExitInstance)
		{
			g_CheckWndInListThreadStarting = FALSE;
			csl1.Unlock();
			csl.Unlock();
			break;
		}
		aWndsCnt= 0;
		aHWndsCnt= 0;
		wndCount = app->m_wndArray.GetSize();
		for(int lpos = 0; lpos < wndCount; lpos++)
		{
			wndDescr = (LPWNDDESCR) app->m_wndArray.GetAt(lpos);
			if(!IsWindow(wndDescr->m_hWnd))
			{
				aWnds[aWndsCnt] = wndDescr->m_hWnd;
				aWndsCnt++;
			}
			else
			{
				if(IsWindow(wndDescr->m_hWnd))
				{
					if(IsWindowVisible(wndDescr->m_hWnd))
					{
						CWndInfoStore* pWIStore;
						int indexFound = 0;

						for(int i = 0; i < app->m_wndInfoStoreArray.GetSize(); i++)
						{
							pWIStore = (CWndInfoStore*) app->m_wndInfoStoreArray.GetAt(i);
							if(wndDescr->m_windowText.Find(pWIStore->m_wndName,0) > -1)
							{
								if(!pWIStore->m_isShowOnActivateApp)
								{
									if(pWIStore->m_progName.IsEmpty())
									{
										indexFound = 1;
										break;
									}
									else
									{
										em.Process(0);
										progName = em.GetModuleName(wndDescr->m_hWnd);
										if(pWIStore->m_progName.CompareNoCase(progName)==0)
										{
											indexFound = 1;
											break;
										}
									}
								}
								else
								{
									if(pWIStore->m_progName.IsEmpty())
									{
										indexFound = -1;
										break;
									}
									else
									{
										em.Process(0);
										progName = em.GetModuleName(wndDescr->m_hWnd);
										if(pWIStore->m_progName.CompareNoCase(progName)==0)
										{
											indexFound = -1;
											break;
										}
									}
								}
							}
						}
						if(indexFound == 0)
						{
							if(app->m_isShowOnActivateApp)
								indexFound = -1;
							else
								indexFound = 1;
						}
						switch(indexFound)
						{
							case 1:
								aHWnds[aHWndsCnt] = wndDescr->m_hWnd;
								aHWndsCnt++;
								break;
							case -1:
								aWnds[aWndsCnt] = wndDescr->m_hWnd;
								aWndsCnt++;
								break;
						}
					}
				}
			}
			GSleep("CheckWndInListThreadProc_3",3);
		}
		l_delayCheckWndInList = g_delayCheckWndInList;
		csl1.Unlock();
		csl.Unlock();
		for(int i1 = 0; i1 < aHWndsCnt; i1++)
			app->m_pMainWnd->SendMessage(WM_APP+10, 0, (LPARAM)aHWnds[i1]);
		for(i1 = 0; i1 < aWndsCnt; i1++)
			app->m_pMainWnd->SendMessage(WM_APP+40, 0, (LPARAM)aWnds[i1]);
		for(UINT i = 0; (i < (l_delayCheckWndInList/100)); i++)
		{
			csl.Lock();
			l_delayCheckWndInList = g_delayCheckWndInList;
			if(g_ExitInstance)
			{	
				csl.Unlock();
				break;
			}
			csl.Unlock();
			GSleep("CheckWndInListThreadProc_100",99);
		}
	}
	return 0;
}

UINT CheckAutoHideWndThreadProc(LPVOID pParam)
{
	CEnumWnd* enumWnd;
	HWND aWnds[1000];
	int aWndsCnt;
	UINT l_delayCheckAutoHideWnd = 0;
	CHideApp* app = (CHideApp*) AfxGetApp();
	CEnumWinModules em;
	CString progName;

	CSingleLock csl(&app->m_CS);
	CSingleLock csl1(&app->m_CSList);
	while(1)
	{
		csl.Lock();
		csl1.Lock();
		if(!g_CheckAutoHideWndThreadRun || g_ExitInstance)
		{
			g_CheckAutoHideWndThreadStarting = FALSE;
			csl1.Unlock();
			csl.Unlock();
			break;
		}
		aWndsCnt= 0;
		enumWnd = new CEnumWnd;
		if(enumWnd->EnumWnd(CEnumWnd::enumSKIPPROGMAN_EMPTY_INV))
		{
			for (int i=0;i < enumWnd->m_wndInfoArray.GetSize();i++)
			{
				LPWNDINFO wndInfo = (LPWNDINFO) enumWnd->m_wndInfoArray.ElementAt(i);
				if(wndInfo->processId != 0)
				{			
					CWndInfoStore* pWIStore;
					int indexFound = -1;

					for(int i = 0; i < app->m_wndInfoStoreArray.GetSize(); i++)
					{
						pWIStore = (CWndInfoStore*) app->m_wndInfoStoreArray.GetAt(i);
						if(pWIStore == NULL) continue;
						if(wndInfo->wText.Find(pWIStore->m_wndName,0) > -1)
						{
							if(pWIStore->m_isHideWhenCreated)
							{
								if(pWIStore->m_progName.IsEmpty())
								{
									indexFound = i;
									break;
								}
								else
								{
									em.Process(0);
									progName = em.GetModuleName(wndInfo->hWnd);
									if(pWIStore->m_progName.CompareNoCase(progName)==0)
									{
										indexFound = i;
										break;
									}
								}
							}
						}
					}
					if(indexFound > -1)
					{
						aWnds[aWndsCnt] = wndInfo->hWnd;
						aWndsCnt++;
					}
				}
				GSleep("CheckWndInListThreadProc_3",3);
			}
		}
		delete enumWnd;
		l_delayCheckAutoHideWnd = g_delayCheckAutoHideWnd;
		csl1.Unlock();
		csl.Unlock();
		for(int i1 = 0; i1 < aWndsCnt; i1++)
			app->m_pMainWnd->SendMessage(WM_APP+10, 0, (LPARAM)aWnds[i1]);
		for(UINT i = 0; (i < (l_delayCheckAutoHideWnd/100)); i++)
		{
			csl.Lock();
			l_delayCheckAutoHideWnd = g_delayCheckAutoHideWnd;
			if(g_ExitInstance)
			{	
				csl.Unlock();
				break;
			}
			csl.Unlock();
			GSleep("CheckWndInListThreadProc_100",99);
		}
	}
	return 0;
}

void CHideApp::ExitInstanceProcess()
{
	CSingleLock csl(&m_CS);
	csl.Lock();
	g_ExitInstance = TRUE;
	csl.Unlock();
}

int CHideApp::ExitInstance() 
{
	BOOL l_OptimizeHiddenThreadStarting = TRUE;
	BOOL l_OptimizeAllThreadStarting = TRUE;
	BOOL l_CheckWndInListThreadStarting = TRUE;
	BOOL l_CheckAutoHideWndThreadStarting = TRUE;
	CSingleLock csl(&m_CS);

	if(p_OptimizeHiddenThread != NULL)
	{
		do
		{
			csl.Lock();
			l_OptimizeHiddenThreadStarting = g_OptimizeHiddenThreadStarting;
			csl.Unlock();
			GSleep("ExitInstance_10", 9);
		}
		while(l_OptimizeHiddenThreadStarting);
	}
	//
	if(p_OptimizeAllThread != NULL)
	{
		do
		{
			csl.Lock();
			l_OptimizeAllThreadStarting = g_OptimizeAllThreadStarting;
			csl.Unlock();
			GSleep("ExitInstance_10",9);
		}
		while(l_OptimizeAllThreadStarting);
	}
	//
	if(p_CheckWndInListThread != NULL)
	{
		do
		{
			csl.Lock();
			l_CheckWndInListThreadStarting = g_CheckWndInListThreadStarting;
			csl.Unlock();
			GSleep("ExitInstance_10",9);
		}
		while(l_CheckWndInListThreadStarting);
	}
	//
	if(p_CheckAutoHideWndThread != NULL)
	{
		do
		{
			csl.Lock();
			l_CheckAutoHideWndThreadStarting = g_CheckAutoHideWndThreadStarting;
			csl.Unlock();
			GSleep("ExitInstance_11",9);
		}
		while(l_CheckAutoHideWndThreadStarting);
	}	
	return CWinApp::ExitInstance();
}


BOOL CHideApp::RestoreOnLog()
{
	INFOSTORE iStore;
	BOOL bResult = FALSE;
	HANDLE hProc;
	DWORD tid = 0, pid = 0;

	do
	{
		bResult = m_infoStore.ReadInfo(-1, &iStore);
		if(bResult)
		{
			if(IsWindow(iStore.hWnd))
			{
				tid = GetWindowThreadProcessId(iStore.hWnd, &pid);
				if( (pid == iStore.dwProcessId) && (tid == iStore.dwThreadId) )
				{
					hProc = OpenProcess(PROCESS_SET_INFORMATION, FALSE, iStore.dwProcessId);
					if(hProc != NULL)
						SetPriorityClass(hProc, iStore.dwPriority);
					CloseHandle(hProc);
					::ShowWindow(iStore.hWnd, SW_SHOW);
					::SetForegroundWindow(iStore.hWnd);
				}
			}
			iStore.dwState = 0;
			m_infoStore.WriteInfo(iStore);
		}
	}
	while(bResult);
	return TRUE;
}

void CHideApp::GetPMThreadProcessId()
{
	HWND hWnd;
	CEnumWnd enumWnd;
	m_PMProcessId = 0;
	m_PMThreadId = 0;


	hWnd = FindWindow("Progman", "Program Manager");
	if(hWnd != NULL)
	{
		m_PMThreadId = GetWindowThreadProcessId(hWnd, &m_PMProcessId);
		return;
	}
	if(enumWnd.EnumWnd(0))
	{
		for (int i=0;i < enumWnd.m_wndInfoArray.GetSize();i++)
		{
			LPWNDINFO wndInfo = (LPWNDINFO) enumWnd.m_wndInfoArray.ElementAt(i);
			if(wndInfo->wText == "Program Manager")
			{
				m_PMProcessId = wndInfo->processId;
				m_PMThreadId = wndInfo->threadId;
				break;
			}
		}
	}
	
}
