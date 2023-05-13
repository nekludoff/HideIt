// WNDPPagePerfomance.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "WNDPPagePerfomance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWNDPPagePerfomance property page

IMPLEMENT_DYNCREATE(CWNDPPagePerfomance, CPropertyPage)

CWNDPPagePerfomance::CWNDPPagePerfomance() : CPropertyPage(CWNDPPagePerfomance::IDD)
{
	//{{AFX_DATA_INIT(CWNDPPagePerfomance)
	m_isChangeProcessPriority = FALSE;
	m_processPriority = -1;
	m_isDontChangeProcessPriorityPM = FALSE;
	//}}AFX_DATA_INIT
}

CWNDPPagePerfomance::~CWNDPPagePerfomance()
{
}

void CWNDPPagePerfomance::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWNDPPagePerfomance)
	DDX_Control(pDX, IDC_CHANGEPRIORITYPM, m_isDontChangeProcessPriorityPMControl);
	DDX_Control(pDX, IDC_PRIORITYGROUP, m_priorityGroup);
	DDX_Control(pDX, IDC_PRIORITY_NORMAL, m_priorityNormalControl);
	DDX_Control(pDX, IDC_PRIORITY_LOW, m_priorityLowControl);
	DDX_Control(pDX, IDC_PROCESSPRIORITY, m_processPriorityControl);
	DDX_Control(pDX, IDC_CHANGEPRIORITY, m_isChangeProcessPriorityControl);
	DDX_Check(pDX, IDC_CHANGEPRIORITY, m_isChangeProcessPriority);
	DDX_Radio(pDX, IDC_PROCESSPRIORITY, m_processPriority);
	DDX_Check(pDX, IDC_CHANGEPRIORITYPM, m_isDontChangeProcessPriorityPM);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWNDPPagePerfomance, CPropertyPage)
	//{{AFX_MSG_MAP(CWNDPPagePerfomance)
	ON_BN_CLICKED(IDC_CHANGEPRIORITY, OnChangepriority)
	ON_BN_CLICKED(IDC_PROCESSPRIORITY, OnProcesspriority)
	ON_BN_CLICKED(IDC_PRIORITY_NORMAL, OnPriorityNormal)
	ON_BN_CLICKED(IDC_PRIORITY_LOW, OnPriorityLow)
	ON_BN_CLICKED(IDC_CHANGEPRIORITYPM, OnChangeprioritypm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWNDPPagePerfomance message handlers
BOOL CWNDPPagePerfomance::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	//	Disable all controls
	if(m_isChangeProcessPriority)
	{
		m_priorityGroup.EnableWindow(TRUE);
		m_processPriorityControl.EnableWindow(TRUE);
		m_priorityNormalControl.EnableWindow(TRUE);
		m_priorityLowControl.EnableWindow(TRUE);
		m_isDontChangeProcessPriorityPMControl.EnableWindow(TRUE);
	}
	else
	{
		m_priorityGroup.EnableWindow(FALSE);
		m_processPriorityControl.EnableWindow(FALSE);
		m_priorityNormalControl.EnableWindow(FALSE);
		m_priorityLowControl.EnableWindow(FALSE);
		m_isDontChangeProcessPriorityPMControl.EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWNDPPagePerfomance::OnChangepriority() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
	if(m_isChangeProcessPriority)
	{
		m_priorityGroup.EnableWindow(TRUE);
		m_processPriorityControl.EnableWindow(TRUE);
		m_priorityNormalControl.EnableWindow(TRUE);
		m_priorityLowControl.EnableWindow(TRUE);
		m_isDontChangeProcessPriorityPMControl.EnableWindow(TRUE);
	}
	else
	{
		m_priorityGroup.EnableWindow(FALSE);
		m_processPriorityControl.EnableWindow(FALSE);
		m_priorityNormalControl.EnableWindow(FALSE);
		m_priorityLowControl.EnableWindow(FALSE);
		m_isDontChangeProcessPriorityPMControl.EnableWindow(FALSE);
	}
}

void CWNDPPagePerfomance::OnProcesspriority() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CWNDPPagePerfomance::OnPriorityNormal() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CWNDPPagePerfomance::OnPriorityLow() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CWNDPPagePerfomance::OnChangeprioritypm() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}
