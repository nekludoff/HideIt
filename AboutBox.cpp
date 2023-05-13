// AboutBox.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "AboutBox.h"
#include "RegDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CAboutBox, CDialog)
/////////////////////////////////////////////////////////////////////////////
// CAboutBox dialog


CAboutBox::CAboutBox(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutBox)
	//}}AFX_DATA_INIT
}


void CAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutBox)
	DDX_Control(pDX, IDC_REGINFO, m_regInfoControl);
	DDX_Control(pDX, IDC_CLIENTINFO, m_clientInfoControl);
	DDX_Control(pDX, IDC_REGISTER, m_registerControl);
	DDX_Control(pDX, IDC_INTERNET, m_internetControl);
	DDX_Control(pDX, IDC_EMAIL, m_eMailControl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutBox, CDialog)
	//{{AFX_MSG_MAP(CAboutBox)
	ON_BN_CLICKED(IDC_REGISTER, OnRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutBox message handlers

BOOL CAboutBox::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_hStaticCursor = AfxGetApp()->LoadCursor(IDC_CURSORHARROW);

	CHideApp* app = (CHideApp*) AfxGetApp();
	if(lstrlen(app->m_userName) > 0)
	{
		CString un = app->m_userName;
		m_registerControl.EnableWindow(FALSE);
		m_registerControl.ShowWindow(SW_HIDE);
		m_regInfoControl.SetWindowText(_T("Registered to:"));
		//m_regInfoControl.SetWindowText(_T("Free version"));
		m_clientInfoControl.SetWindowText(un);
	} else
	{
		char buf[100];
		wsprintf(buf, "You have %d day(s) left", app->m_trialDaysLeft);
		CString dl = buf;
		dl.TrimRight();
		dl.TrimLeft();
		m_clientInfoControl.SetWindowText(dl);
	}

	m_eMailControl.SetURL(_T("mailto:support@kamtre.ru"));
	m_internetControl.SetURL(_T("https://www.kamtre.ru"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutBox::OnRegister() 
{
//	ShowWindow(SW_HIDE);
//	CRegDialog dlgReg;
//	dlgReg.DoModal();
//	EndDialog(IDOK);
}
