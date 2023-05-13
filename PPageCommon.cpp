// PPageCommon.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "PPageCommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPageCommon property page

IMPLEMENT_DYNCREATE(CPPageCommon, CPropertyPage)

CPPageCommon::CPPageCommon() : CPropertyPage(CPPageCommon::IDD)
{
	//{{AFX_DATA_INIT(CPPageCommon)
	m_isProcessInvisibleWindows = FALSE;
	m_isSplashWindow = FALSE;
	m_isAddHideAll = FALSE;
	m_isAutoStart = FALSE;
	m_isSoftExit = FALSE;
	m_isAddHideIt = FALSE;
	//}}AFX_DATA_INIT
}

CPPageCommon::~CPPageCommon()
{

}

void CPPageCommon::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPageCommon)
	DDX_Check(pDX, IDC_PROCESSINVISIBLEWND, m_isProcessInvisibleWindows);
	DDX_Check(pDX, IDC_SPLASHWINDOW, m_isSplashWindow);
	DDX_Check(pDX, IDC_ADDHIDEALL, m_isAddHideAll);
	DDX_Check(pDX, IDC_AUTOSTART, m_isAutoStart);
	DDX_Check(pDX, IDC_SOFTEXIT, m_isSoftExit);
	DDX_Check(pDX, IDC_ADDHIDE, m_isAddHideIt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPageCommon, CPropertyPage)
	//{{AFX_MSG_MAP(CPPageCommon)
	ON_BN_CLICKED(IDC_PROCESSINVISIBLEWND, OnProcessinvisiblewnd)
	ON_BN_CLICKED(IDC_SPLASHWINDOW, OnSplashwindow)
	ON_BN_CLICKED(IDC_ADDHIDEALL, OnAddhideall)
	ON_BN_CLICKED(IDC_AUTOSTART, OnAutostart)
	ON_BN_CLICKED(IDC_SOFTEXIT, OnSoftexit)
	ON_BN_CLICKED(IDC_ADDHIDE, OnAddhide)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPageCommon message handlers

void CPPageCommon::OnProcessinvisiblewnd() 
{
	UpdateData(TRUE);
	SetModified(TRUE);	
}

void CPPageCommon::OnSplashwindow() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageCommon::OnAddhideall() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageCommon::OnAutostart() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}


void CPPageCommon::OnSoftexit() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageCommon::OnAddhide() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}
