// TrialDialog.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "TrialDialog.h"
#include "RegDialog.h"
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrialDialog dialog


CTrialDialog::CTrialDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTrialDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrialDialog)
	//}}AFX_DATA_INIT
}


void CTrialDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrialDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrialDialog, CDialog)
	//{{AFX_MSG_MAP(CTrialDialog)
	ON_BN_CLICKED(IDC_BUYNOW, OnBuynow)
	ON_BN_CLICKED(IDC_REG_BUTTON, OnRegButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrialDialog message handlers

void CTrialDialog::OnBuynow() 
{
	ShellExecute(NULL, NULL, "http://www.rokelasoft.com/register.html", NULL, NULL, SW_SHOWNORMAL);
}

void CTrialDialog::OnRegButton() 
{
	ShowWindow(SW_HIDE);
	CRegDialog dlgReg;
	dlgReg.DoModal();
	EndDialog(IDOK);
}

BOOL CTrialDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRegKey regKey;
	if(regKey.Create(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run")==ERROR_SUCCESS)
		regKey.DeleteValue(AfxGetAppName());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
