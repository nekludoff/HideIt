// PPageWndMain.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "PPageWndMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPageWndMain property page

IMPLEMENT_DYNCREATE(CPPageWndMain, CPropertyPage)

CPPageWndMain::CPPageWndMain() : CPropertyPage(CPPageWndMain::IDD)
{
	//{{AFX_DATA_INIT(CPPageWndMain)
	m_wndExeName = _T("");
	m_wndName = _T("");
	m_wndListIndex = 0;
	//}}AFX_DATA_INIT
}

CPPageWndMain::~CPPageWndMain()
{
}

void CPPageWndMain::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPageWndMain)
	DDX_Control(pDX, IDC_WNDICON, m_wndIcon);
	DDX_Text(pDX, IDC_EXENAME, m_wndExeName);
	DDX_Text(pDX, IDC_WNDNAME, m_wndName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPageWndMain, CPropertyPage)
	//{{AFX_MSG_MAP(CPPageWndMain)
	ON_EN_CHANGE(IDC_WNDNAME, OnChangeWndname)
	ON_BN_CLICKED(IDC_CLEARBUTTON, OnClearbutton)
	ON_BN_CLICKED(IDC_RESTOREBUTTON, OnRestorebutton)
	ON_BN_CLICKED(IDC_CLEAREXEBUTTON, OnClearexebutton)
	ON_BN_CLICKED(IDC_FINDBUTTON, OnFindbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPageWndMain message handlers

void CPPageWndMain::OnChangeWndname() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

BOOL CPPageWndMain::OnInitDialog() 
{
	CHideApp* app = (CHideApp*) AfxGetApp();

	CPropertyPage::OnInitDialog();

	HICON hIcon = NULL;

	if(app->m_wndInfoStoreImageList.GetSafeHandle() == NULL)
		app->m_wndInfoStoreImageList.Create(16, 16, ILC_COLOR4|ILC_MASK, 8, 4);
	if((app->m_wndInfoStoreImageList.GetImageCount()-1)>=m_wndListIndex)
		hIcon = app->m_wndInfoStoreImageList.ExtractIcon(m_wndListIndex);
	if(hIcon == NULL)
		app->LoadIcon(IDI_EMPTY);
	if(app->m_wndInfoStoreImageList.GetImageCount() != 0)
		app->m_wndInfoStoreImageList.Replace(m_wndListIndex, hIcon);
	else
		app->m_wndInfoStoreImageList.Add(hIcon);
	m_wndIcon.SetIcon(hIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPPageWndMain::OnClearbutton() 
{
	CHideApp* app = (CHideApp*) AfxGetApp();

	app->m_wndInfoStoreImageList.Replace(m_wndListIndex, app->LoadIcon(IDI_EMPTY));
	m_wndIcon.SetIcon(app->m_wndInfoStoreImageList.ExtractIcon(m_wndListIndex));
}

void CPPageWndMain::OnRestorebutton() 
{
	HICON bigWndIcon = NULL;
	HICON hIcon = NULL;
	CHideApp* app = (CHideApp*) AfxGetApp();

	::ExtractIconEx(m_wndExeName, 0, &bigWndIcon, &hIcon, 1);
	if(hIcon == NULL)
		app->LoadIcon(IDI_EMPTY);
//	if(app->m_wndInfoStoreImageList.GetImageCount() <= 0)
//		app->m_wndInfoStoreImageList.Add(hIcon);
	int res = app->m_wndInfoStoreImageList.Replace(m_wndListIndex, hIcon);
	m_wndIcon.SetIcon(app->m_wndInfoStoreImageList.ExtractIcon(m_wndListIndex));
}

void CPPageWndMain::OnClearexebutton() 
{
	CHideApp* app = (CHideApp*) AfxGetApp();

	UpdateData(TRUE);
	SetModified(TRUE);
	m_wndExeName.Empty();
	app->m_wndInfoStoreImageList.Replace(m_wndListIndex, app->LoadIcon(IDI_EMPTY));
	m_wndIcon.SetIcon(app->m_wndInfoStoreImageList.ExtractIcon(m_wndListIndex));
	UpdateData(FALSE);
}

void CPPageWndMain::OnFindbutton() 
{
	CFileDialog* cf;
	cf = new CFileDialog(TRUE);
	int res = cf->DoModal();
	if(res = IDOK)
	{
		UpdateData(TRUE);
		SetModified(TRUE);
		m_wndExeName = cf->GetPathName();
		OnRestorebutton();
		UpdateData(FALSE);
	}
	delete cf;
}
