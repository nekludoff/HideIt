// RegDialog.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "RegDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegDialog dialog


CRegDialog::CRegDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRegDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegDialog)
	m_regKey = _T("");
	//}}AFX_DATA_INIT
}


void CRegDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegDialog)
	DDX_Text(pDX, IDC_REGNUM_EDIT, m_regKey);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegDialog, CDialog)
	//{{AFX_MSG_MAP(CRegDialog)
	ON_BN_CLICKED(IDC_PASTE, OnPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegDialog message handlers

void CRegDialog::OnPaste() 
{
	::SendMessage(::GetDlgItem(this->m_hWnd,IDC_REGNUM_EDIT), WM_PASTE, 0,0);
}

void CRegDialog::OnOK() 
{
	UpdateData(TRUE);
	if (!m_regKey.IsEmpty()) 
	{
		CHideApp* app = (CHideApp*) AfxGetApp();	
		if( app->WriteProfileString(_T(""), _T("Key"), m_regKey) )
			AfxMessageBox("Thank you for registration! Please restart the application.");
	}
	CDialog::OnOK();
}
