// PPageAppMenu.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "PPageAppMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPageAppMenu property page

IMPLEMENT_DYNCREATE(CPPageAppMenu, CPropertyPage)

CPPageAppMenu::CPPageAppMenu() : CPropertyPage(CPPageAppMenu::IDD)
{
	//{{AFX_DATA_INIT(CPPageAppMenu)
	m_isSmallIcons = FALSE;
	m_isTrimCaption = FALSE;
	m_trimCaptionChars = _T("");
	m_isAddHideAllApp = FALSE;
	m_isAddShowAllApp = FALSE;
	m_isExtractIconFromFile = FALSE;
	//}}AFX_DATA_INIT
}

CPPageAppMenu::~CPPageAppMenu()
{
}

void CPPageAppMenu::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPageAppMenu)
	DDX_Control(pDX, IDC_TRIMCHARS, m_trimChars);
	DDX_Check(pDX, IDC_MENU_ICONTYPE, m_isSmallIcons);
	DDX_Check(pDX, IDC_TRIMCAPTION, m_isTrimCaption);
	DDX_Text(pDX, IDC_TRIMCHARS, m_trimCaptionChars);
	DDV_MaxChars(pDX, m_trimCaptionChars, 3);
	DDX_Check(pDX, IDC_ADDHIDEALL, m_isAddHideAllApp);
	DDX_Check(pDX, IDC_ADDSHOWALL, m_isAddShowAllApp);
	DDX_Check(pDX, IDC_EXTRACTICONFROMEXE, m_isExtractIconFromFile);
	//}}AFX_DATA_MAP
//	DDV_TrimChars(pDX, m_trimCaptionChars, &m_trimChars);
}


BEGIN_MESSAGE_MAP(CPPageAppMenu, CPropertyPage)
	//{{AFX_MSG_MAP(CPPageAppMenu)
	ON_BN_CLICKED(IDC_MENU_ICONTYPE, OnMenuIcontype)
	ON_BN_CLICKED(IDC_TRIMCAPTION, OnTrimcaption)
	ON_EN_KILLFOCUS(IDC_TRIMCHARS, OnKillfocusTrimchars)
	ON_EN_CHANGE(IDC_TRIMCHARS, OnChangeTrimchars)
	ON_BN_CLICKED(IDC_ADDSHOWALL, OnAddshowall)
	ON_BN_CLICKED(IDC_ADDHIDEALL, OnAddhideall)
	ON_BN_CLICKED(IDC_EXTRACTICONFROMEXE, OnExtracticonfromexe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPageAppMenu message handlers

void CPPageAppMenu::OnMenuIcontype() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageAppMenu::OnTrimcaption() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
	if(m_isTrimCaption)
		m_trimChars.EnableWindow(TRUE);
	else
		m_trimChars.EnableWindow(FALSE);
}

BOOL CPPageAppMenu::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	UpdateData(TRUE);
	if(m_isTrimCaption)
		m_trimChars.EnableWindow(TRUE);
	else
		m_trimChars.EnableWindow(FALSE);
	m_trimChars.SetWindowText(m_trimCaptionChars);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPPageAppMenu::OnKillfocusTrimchars() 
{
	CString st;
	m_trimChars.GetWindowText(st);
	int i = atoi(st);
	if((i < 10)||(i > 256))
	{
		AfxMessageBox("Enter integer value between 10 and 256",MB_OK|MB_ICONSTOP);
		m_trimChars.SetFocus();
	} else
	{
		char buffer[3];
		_itoa( i, buffer, 10 );
		m_trimCaptionChars.Empty();
		m_trimCaptionChars = buffer;
		m_trimChars.SetWindowText(m_trimCaptionChars);
	}
}

void CPPageAppMenu::OnChangeTrimchars() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}


void CPPageAppMenu::OnAddshowall() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPageAppMenu::OnAddhideall() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}



void CPPageAppMenu::OnExtracticonfromexe() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void PASCAL DDV_TrimChars(CDataExchange* pDX, CString str, CEdit* control)
{
	if(pDX->m_bSaveAndValidate)
	{
		int i = atoi(str);
		if((i < 10)||(i > 256))
		{
			AfxMessageBox("Enter integer value between 10 and 256",MB_OK|MB_ICONSTOP);
			control->SetFocus();
			pDX->Fail();
		} 
		else
		{
/*
			char buffer[3];
			_itoa( i, buffer, 10 );
			m_trimCaptionChars.Empty();
			m_trimCaptionChars = buffer;
			m_trimChars.SetWindowText(m_trimCaptionChars);
*/
		}
	}
}
