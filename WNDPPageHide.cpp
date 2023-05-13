// WNDPPageHide.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "WNDPPageHide.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWNDPPageHide property page

IMPLEMENT_DYNCREATE(CWNDPPageHide, CPropertyPage)

CWNDPPageHide::CWNDPPageHide() : CPropertyPage(CWNDPPageHide::IDD)
{
	//{{AFX_DATA_INIT(CWNDPPageHide)
	m_isHideWhenMinimize = FALSE;
	m_isHideByRMouse = FALSE;
	m_isShowOnActivateApp = FALSE;
	m_hideTo = -1;
	m_hideMethod = -1;
	m_isActivateNextWindow = FALSE;
	m_isHideByDblRMouse = FALSE;
	m_isHideWhenCreated = FALSE;
	//}}AFX_DATA_INIT
}

CWNDPPageHide::~CWNDPPageHide()
{
}

void CWNDPPageHide::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWNDPPageHide)
	DDX_Control(pDX, IDC_SHOWAPP, m_isShowOnActivateAppControl);
	DDX_Control(pDX, IDC_HIDE_CREATE, m_isHideWhenCreatedControl);
	DDX_Check(pDX, IDC_HIDE_MINIMIZE, m_isHideWhenMinimize);
	DDX_Check(pDX, IDC_MOUSEHIDE, m_isHideByRMouse);
	DDX_Check(pDX, IDC_SHOWAPP, m_isShowOnActivateApp);
	DDX_Radio(pDX, IDC_RADIOHIDETO, m_hideTo);
	DDX_Radio(pDX, IDC_RADIOHIDEMETHOD, m_hideMethod);
	DDX_Check(pDX, IDC_ACTIVATENEXT, m_isActivateNextWindow);
	DDX_Check(pDX, IDC_MOUSEHIDEDBL, m_isHideByDblRMouse);
	DDX_Check(pDX, IDC_HIDE_CREATE, m_isHideWhenCreated);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWNDPPageHide, CPropertyPage)
	//{{AFX_MSG_MAP(CWNDPPageHide)
	ON_BN_CLICKED(IDC_HIDE_MINIMIZE, OnHideMinimize)
	ON_BN_CLICKED(IDC_MOUSEHIDE, OnMousehide)
	ON_BN_CLICKED(IDC_SHOWAPP, OnShowapp)
	ON_BN_CLICKED(IDC_RADIOHIDETO, OnRadiohideto)
	ON_BN_CLICKED(IDC_RADIOHIDETO_MENUTRAY, OnRadiohidetoMenutray)
	ON_BN_CLICKED(IDC_RADIOHIDETO_TRAY, OnRadiohidetoTray)
	ON_BN_CLICKED(IDC_HIDEALLWNDAPP, OnHideallwndapp)
	ON_BN_CLICKED(IDC_RADIOHIDEMETHOD, OnRadiohidemethod)
	ON_BN_CLICKED(IDC_RADIOHIDEMETHOD2, OnRadiohidemethod2)
	ON_BN_CLICKED(IDC_RADIOHIDEMETHOD3, OnRadiohidemethod3)
	ON_BN_CLICKED(IDC_ACTIVATENEXT, OnActivatenext)
	ON_BN_CLICKED(IDC_MOUSEHIDEDBL, OnMousehidedbl)
	ON_BN_CLICKED(IDC_RADIOHIDETO_PRIVATEMENU, OnRadiohidetoPrivatemenu)
	ON_BN_CLICKED(IDC_HIDE_CREATE, OnHideCreate)
	ON_BN_CLICKED(IDC_RADIOHIDETO_PRIVATEMENUCOMMON, OnRadiohidetoPrivatemenucommon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWNDPPageHide message handlers
void CWNDPPageHide::OnHideMinimize() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}


void CWNDPPageHide::OnMousehide() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CWNDPPageHide::OnMousehidedbl() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

BOOL CWNDPPageHide::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	//
	if(m_isShowOnActivateApp)
	{
		m_isHideWhenCreated = FALSE;
		m_isHideWhenCreatedControl.EnableWindow(FALSE);
	}
	else
		m_isHideWhenCreatedControl.EnableWindow(TRUE);
	//
	if(m_isHideWhenCreated)
	{
		m_isShowOnActivateApp = FALSE;
		m_isShowOnActivateAppControl.EnableWindow(FALSE);
	}
	else
		m_isShowOnActivateAppControl.EnableWindow(TRUE);
	//	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWNDPPageHide::OnShowapp() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
	if(m_isShowOnActivateApp)
	{
		m_isHideWhenCreated = FALSE;
		m_isHideWhenCreatedControl.EnableWindow(FALSE);
	}
	else
		m_isHideWhenCreatedControl.EnableWindow(TRUE);
	UpdateData(FALSE);
}

void CWNDPPageHide::OnRadiohideto() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CWNDPPageHide::OnRadiohidetoMenutray() 
{
	UpdateData(TRUE);
	SetModified(TRUE);	
}

void CWNDPPageHide::OnRadiohidetoTray() 
{
	UpdateData(TRUE);
	SetModified(TRUE);	
}

void CWNDPPageHide::OnHideallwndapp() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CWNDPPageHide::OnRadiohidemethod() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CWNDPPageHide::OnRadiohidemethod2() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CWNDPPageHide::OnRadiohidemethod3() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CWNDPPageHide::OnActivatenext() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CWNDPPageHide::OnRadiohidetoPrivatemenu() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CWNDPPageHide::OnHideCreate() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
	if(m_isHideWhenCreated)
	{
		m_isShowOnActivateApp = FALSE;
		m_isShowOnActivateAppControl.EnableWindow(FALSE);
	}
	else
		m_isShowOnActivateAppControl.EnableWindow(TRUE);
	UpdateData(FALSE);
}

void CWNDPPageHide::OnRadiohidetoPrivatemenucommon() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}
