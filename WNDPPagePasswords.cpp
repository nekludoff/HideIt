// WNDPagePasswords.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "WNDPPagePasswords.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWNDPPagePasswords property page

IMPLEMENT_DYNCREATE(CWNDPPagePasswords, CPropertyPage)

CWNDPPagePasswords::CWNDPPagePasswords() : CPropertyPage(CWNDPPagePasswords::IDD)
{
	//{{AFX_DATA_INIT(CWNDPPagePasswords)
	m_masterPassword = _T("012345678901234");
	m_confirmMasterPassword = _T("012345678901234");
	m_oldMasterPassword = _T("012345678901234");
	//}}AFX_DATA_INIT
}

CWNDPPagePasswords::~CWNDPPagePasswords()
{
}

void CWNDPPagePasswords::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWNDPPagePasswords)
	DDX_Control(pDX, IDC_MASTERPWD, m_masterPasswordControl);
	DDX_Control(pDX, IDC_MASTERPWDCONFIRM, m_confirmMasterPasswordControl);
	DDX_Text(pDX, IDC_MASTERPWD, m_masterPassword);
	DDX_Text(pDX, IDC_MASTERPWDCONFIRM, m_confirmMasterPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWNDPPagePasswords, CPropertyPage)
	//{{AFX_MSG_MAP(CWNDPPagePasswords)
	ON_EN_CHANGE(IDC_MASTERPWD, OnChangeMasterpwd)
	ON_EN_CHANGE(IDC_MASTERPWDCONFIRM, OnChangeMasterpwdconfirm)
	ON_EN_KILLFOCUS(IDC_MASTERPWDCONFIRM, OnKillfocusMasterpwdconfirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWNDPPagePasswords message handlers
BOOL CWNDPPagePasswords::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWNDPPagePasswords::OnChangeMasterpwd() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
	m_confirmMasterPassword = _T("");
	UpdateData(FALSE);
}

void CWNDPPagePasswords::OnChangeMasterpwdconfirm() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CWNDPPagePasswords::OnKillfocusMasterpwdconfirm() 
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
