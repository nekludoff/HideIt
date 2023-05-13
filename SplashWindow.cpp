// SplashWindow.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "SplashWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplashWindow dialog


CSplashWindow::CSplashWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSplashWindow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSplashWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplashWindow)
	DDX_Control(pDX, IDC_SPLASHICON, m_splashIcon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSplashWindow, CDialog)
	//{{AFX_MSG_MAP(CSplashWindow)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplashWindow message handlers


void CSplashWindow::GSleep(LPCTSTR lpName, UINT uMillisec)
{
	HANDLE h = ::CreateEvent(NULL, FALSE, FALSE, lpName);
	::WaitForSingleObject(h, uMillisec);
	::CloseHandle(h);
}

void CSplashWindow::SetSplashTimerUp(UINT msec)
{	
	ShowWindow(SW_SHOW);
	UpdateWindow();
	GSleep("CSplashWindow_1", msec);
	ShowWindow(SW_HIDE);
}
