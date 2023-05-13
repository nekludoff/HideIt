// PPagePasswords.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "PPagePasswords.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPagePasswords property page

IMPLEMENT_DYNCREATE(CPPagePasswords, CPropertyPage)

CPPagePasswords::CPPagePasswords() : CPropertyPage(CPPagePasswords::IDD)
{
	//{{AFX_DATA_INIT(CPPagePasswords)
	m_masterPassword = _T("012345678901234");
	m_confirmMasterPassword = _T("012345678901234");
	m_isPwdUseExit = FALSE;
	m_isPwdUseSettings = FALSE;
	m_isPwdUseShowMenu = FALSE;
	m_isPwdUseShowPrivateMenu = FALSE;
	m_isPwdUseShowWindow = FALSE;
	m_oldMasterPassword = _T("012345678901234");
	m_isPwdUseWndSettings = FALSE;
	//}}AFX_DATA_INIT
}

CPPagePasswords::~CPPagePasswords()
{
}

void CPPagePasswords::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPagePasswords)
	DDX_Control(pDX, IDC_PWDUSESHOWWINDOW, m_isPwdUseShowWindowControl);
	DDX_Control(pDX, IDC_MASTERPWD, m_masterPasswordControl);
	DDX_Control(pDX, IDC_MASTERPWDCONFIRM, m_confirmMasterPasswordControl);
	DDX_Control(pDX, IDC_MASTERBOX, m_masterBox);
	DDX_Control(pDX, IDC_GROUPBOX, m_groupBox);
	DDX_Text(pDX, IDC_MASTERPWD, m_masterPassword);
	DDX_Text(pDX, IDC_MASTERPWDCONFIRM, m_confirmMasterPassword);
	DDX_Check(pDX, IDC_PWDUSEEXIT, m_isPwdUseExit);
	DDX_Check(pDX, IDC_PWDUSESETTINGS, m_isPwdUseSettings);
	DDX_Check(pDX, IDC_PWDUSESHOWMENU, m_isPwdUseShowMenu);
	DDX_Check(pDX, IDC_PWDUSESHOWPRIVATEMENU, m_isPwdUseShowPrivateMenu);
	DDX_Check(pDX, IDC_PWDUSESHOWWINDOW, m_isPwdUseShowWindow);
	DDX_Check(pDX, IDC_PWDUSEWNDSETTINGS, m_isPwdUseWndSettings);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPagePasswords, CPropertyPage)
	//{{AFX_MSG_MAP(CPPagePasswords)
	ON_EN_CHANGE(IDC_MASTERPWD, OnChangeMasterpwd)
	ON_EN_CHANGE(IDC_MASTERPWDCONFIRM, OnChangeMasterpwdconfirm)
	ON_BN_CLICKED(IDC_PWDUSEEXIT, OnPwduseexit)
	ON_BN_CLICKED(IDC_PWDUSESETTINGS, OnPwdusesettings)
	ON_BN_CLICKED(IDC_PWDUSESHOWMENU, OnPwduseshowmenu)
	ON_BN_CLICKED(IDC_PWDUSESHOWPRIVATEMENU, OnPwduseshowprivatemenu)
	ON_BN_CLICKED(IDC_PWDUSESHOWWINDOW, OnPwduseshowwindow)
	ON_EN_KILLFOCUS(IDC_MASTERPWDCONFIRM, OnKillfocusMasterpwdconfirm)
	ON_BN_CLICKED(IDC_PWDUSEWNDSETTINGS, OnPwdusewndsettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPagePasswords message handlers

BOOL CPPagePasswords::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	if(!m_isPwdUseExit)
	{
		m_isPwdUseShowWindow = FALSE;
		m_isPwdUseShowWindowControl.EnableWindow(FALSE);
	}
	else
		m_isPwdUseShowWindowControl.EnableWindow(TRUE);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPPagePasswords::OnChangeMasterpwd() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
	m_confirmMasterPassword = _T("");
	UpdateData(FALSE);
}

void CPPagePasswords::OnChangeMasterpwdconfirm() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPagePasswords::OnPwduseexit() 
{
	UpdateData(TRUE);
	if(!m_isPwdUseExit)
	{
		m_isPwdUseShowWindow = FALSE;
		m_isPwdUseShowWindowControl.EnableWindow(FALSE);
	}
	else
		m_isPwdUseShowWindowControl.EnableWindow(TRUE);
	UpdateData(FALSE);
	SetModified(TRUE);
}

void CPPagePasswords::OnPwdusesettings() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPagePasswords::OnPwdusewndsettings() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPagePasswords::OnPwduseshowmenu() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPagePasswords::OnPwduseshowprivatemenu() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPagePasswords::OnPwduseshowwindow() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPagePasswords::OnKillfocusMasterpwdconfirm() 
{
	UpdateData(TRUE);
	if(m_masterPassword != m_confirmMasterPassword)
	{
		AfxMessageBox("The password was not correctly confirmed. Please ensure that the password and confirmation match exactly.");
		m_masterPassword = _T("");
		m_confirmMasterPassword = _T("");
		UpdateData(FALSE);
		m_masterPasswordControl.SetFocus();
	}
}
