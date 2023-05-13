// PPageHide.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "PPageHide.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPageHide property page

IMPLEMENT_DYNCREATE(CPPageHide, CPropertyPage)

CPPageHide::CPPageHide() : CPropertyPage(CPPageHide::IDD)
{
	//{{AFX_DATA_INIT(CPPageHide)
	m_isHideWhenMinimize = FALSE;
	m_isHideByRMouse = FALSE;
	m_isShowOnActivateApp = FALSE;
	m_hideTo = -1;
	m_hideMethod = -1;
	m_isActivateNextWindow = FALSE;
	m_isHideByDblRMouse = FALSE;
	m_isShowPopupWndMenu = FALSE;
	//}}AFX_DATA_INIT
}

CPPageHide::~CPPageHide()
{

}

void CPPageHide::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPageHide)
	DDX_Check(pDX, IDC_HIDE_MINIMIZE, m_isHideWhenMinimize);
	DDX_Check(pDX, IDC_MOUSEHIDE, m_isHideByRMouse);
	DDX_Check(pDX, IDC_SHOWAPP, m_isShowOnActivateApp);
	DDX_Radio(pDX, IDC_RADIOHIDETO, m_hideTo);
	DDX_Radio(pDX, IDC_RADIOHIDEMETHOD, m_hideMethod);
	DDX_Check(pDX, IDC_ACTIVATENEXT, m_isActivateNextWindow);
	DDX_Check(pDX, IDC_MOUSEHIDEDBL, m_isHideByDblRMouse);
	DDX_Check(pDX, IDC_WNDMENU, m_isShowPopupWndMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPageHide, CPropertyPage)
	//{{AFX_MSG_MAP(CPPageHide)
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
	ON_BN_CLICKED(IDC_WNDMENU, OnWndmenu)
	ON_BN_CLICKED(IDC_RADIOHIDETO_PRIVATEMENUCOMMON, OnRadiohidetoPrivatemenucommon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPageHide message handlers

void CPPageHide::OnHideMinimize() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}


void CPPageHide::OnMousehide() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageHide::OnMousehidedbl() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

BOOL CPPageHide::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPPageHide::OnShowapp() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageHide::OnRadiohideto() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageHide::OnRadiohidetoMenutray() 
{
	UpdateData(TRUE);
	SetModified(TRUE);	
}

void CPPageHide::OnRadiohidetoTray() 
{
	UpdateData(TRUE);
	SetModified(TRUE);	
}

void CPPageHide::OnHideallwndapp() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageHide::OnRadiohidemethod() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageHide::OnRadiohidemethod2() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageHide::OnRadiohidemethod3() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageHide::OnActivatenext() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageHide::OnRadiohidetoPrivatemenu() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}


void CPPageHide::OnWndmenu() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageHide::OnRadiohidetoPrivatemenucommon() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}
