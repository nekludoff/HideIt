// PPagePerfomance.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "PPagePerfomance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPagePerfomance property page

IMPLEMENT_DYNCREATE(CPPagePerfomance, CPropertyPage)

CPPagePerfomance::CPPagePerfomance() : CPropertyPage(CPPagePerfomance::IDD)
{
	//{{AFX_DATA_INIT(CPPagePerfomance)
	m_isReduceProcessMemory = FALSE;
	m_isReduceAllProcessesMemory = FALSE;
	m_isChangeProcessPriority = FALSE;
	m_processPriority = -1;
	m_isDontChangeProcessPriorityPM = FALSE;
	//}}AFX_DATA_INIT
	m_delayOptimHiddenApp = 0;
	m_delayOptimHiddenApp_index = 3;
	m_delayOptimAllApp = 0;
	m_delayOptimAllApp_index = 3;
	m_delayA[0] = 10;
	m_delayA[1] = 100;
	m_delayA[2] = 1000;
	m_delayA[3] = 10000;
	m_delayA[4] = 60000;
	m_delayA[5] = 600000;
	m_delayA[6] = 1800000;
	m_delayA[7] = 3600000;
	m_delayA[8] = 21600000;
	m_delayA[9] = 43200000;
	m_delayA[10] = 86400000;
}

CPPagePerfomance::~CPPagePerfomance()
{
}

void CPPagePerfomance::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPagePerfomance)
	DDX_Control(pDX, IDC_CHANGEPRIORITYPM, m_isDontChangeProcessPriorityPMControl);
	DDX_Control(pDX, IDC_PRIORITYGROUP, m_priorityGroup);
	DDX_Control(pDX, IDC_MEMOPTGROUP, m_memOptGroup);
	DDX_Control(pDX, IDC_REDUCEMEMORY, m_isReduceProcessMemoryControl);
	DDX_Control(pDX, IDC_PRIORITY_NORMAL, m_priorityNormalControl);
	DDX_Control(pDX, IDC_PRIORITY_LOW, m_priorityLowControl);
	DDX_Control(pDX, IDC_PROCESSPRIORITY, m_processPriorityControl);
	DDX_Control(pDX, IDC_CHANGEPRIORITY, m_isChangeProcessPriorityControl);
	DDX_Control(pDX, IDC_STATICOPT, m_delayOptimHiddenAppTextControl);
	DDX_Control(pDX, IDC_STATICOPTALL, m_delayOptimAllAppTextControl);
	DDX_Control(pDX, IDC_REDUCEALLMEMORY, m_isReduceAllProcessesMemoryControl);
	DDX_Control(pDX, IDC_DELAYCOMBOHIDDENALL, m_delayOptimAllAppControl);
	DDX_Control(pDX, IDC_DELAYCOMBOHIDDEN, m_delayOptimHiddenAppControl);
	DDX_Check(pDX, IDC_REDUCEMEMORY, m_isReduceProcessMemory);
	DDX_Check(pDX, IDC_REDUCEALLMEMORY, m_isReduceAllProcessesMemory);
	DDX_Check(pDX, IDC_CHANGEPRIORITY, m_isChangeProcessPriority);
	DDX_Radio(pDX, IDC_PROCESSPRIORITY, m_processPriority);
	DDX_Check(pDX, IDC_CHANGEPRIORITYPM, m_isDontChangeProcessPriorityPM);
	//}}AFX_DATA_MAP
	if(pDX->m_bSaveAndValidate)
		PrepareVariables();
}


BEGIN_MESSAGE_MAP(CPPagePerfomance, CPropertyPage)
	//{{AFX_MSG_MAP(CPPagePerfomance)
	ON_BN_CLICKED(IDC_REDUCEMEMORY, OnReducememory)
	ON_BN_CLICKED(IDC_REDUCEALLMEMORY, OnReduceallmemory)
	ON_CBN_SELCHANGE(IDC_DELAYCOMBOHIDDEN, OnSelchangeDelaycombohidden)
	ON_CBN_SELCHANGE(IDC_DELAYCOMBOHIDDENALL, OnSelchangeDelaycombohiddenall)
	ON_BN_CLICKED(IDC_CHANGEPRIORITY, OnChangepriority)
	ON_BN_CLICKED(IDC_PROCESSPRIORITY, OnProcesspriority)
	ON_BN_CLICKED(IDC_PRIORITY_NORMAL, OnPriorityNormal)
	ON_BN_CLICKED(IDC_PRIORITY_LOW, OnPriorityLow)
	ON_BN_CLICKED(IDC_CHANGEPRIORITYPM, OnChangeprioritypm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPagePerfomance message handlers

void CPPagePerfomance::OnReducememory() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
	CHideApp* app = (CHideApp*) AfxGetApp();	
	if(m_isReduceProcessMemory)
	{
		m_delayOptimHiddenAppControl.EnableWindow(TRUE);
		m_delayOptimHiddenAppTextControl.EnableWindow(TRUE);
	}
	else
	{
		m_delayOptimHiddenAppControl.EnableWindow(FALSE);
		m_delayOptimHiddenAppTextControl.EnableWindow(FALSE);
	}
}

void CPPagePerfomance::OnReduceallmemory() 
{
	UpdateData(TRUE);
	SetModified(TRUE);	
	CHideApp* app = (CHideApp*) AfxGetApp();
	if(m_isReduceAllProcessesMemory)
	{
		m_delayOptimAllAppControl.EnableWindow(TRUE);
		m_delayOptimAllAppTextControl.EnableWindow(TRUE);
	}
	else
	{
		m_delayOptimAllAppControl.EnableWindow(FALSE);
		m_delayOptimAllAppTextControl.EnableWindow(FALSE);
	}
}

BOOL CPPagePerfomance::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CHideApp* app = (CHideApp*) AfxGetApp();	
	CString cs[11];
	int i = 0;

	cs[0] = "10 msec.";
	cs[1] = "100 msec.";
	cs[2] = "1 sec.";	
	cs[3] = "10 sec.";
	cs[4] = "1 min.";
	cs[5] = "10 min.";
	cs[6] = "30 min.";
	cs[7] = "1 hour";
	cs[8] = "6 hours";
	cs[9] = "12 hours";
	cs[10] = "24 hours";

	
	for(i = 0; i < 11; i++)
	{
		m_delayOptimHiddenAppControl.AddString(cs[i]);
		m_delayOptimAllAppControl.AddString(cs[i]);
	}
	m_delayOptimHiddenAppControl.SetCurSel(m_delayOptimHiddenApp_index);
	m_delayOptimAllAppControl.SetCurSel(m_delayOptimAllApp_index);
	//	Disable all controls
	m_memOptGroup.EnableWindow(FALSE);
	m_isReduceProcessMemoryControl.EnableWindow(FALSE);
	m_isReduceAllProcessesMemoryControl.EnableWindow(FALSE);
	m_delayOptimHiddenAppControl.EnableWindow(FALSE);
	m_delayOptimHiddenAppTextControl.EnableWindow(FALSE);
	m_delayOptimAllAppControl.EnableWindow(FALSE);
	m_delayOptimAllAppTextControl.EnableWindow(FALSE);

#ifndef _DEBUG
	if(app->m_osi.IsNT())
#endif
	{
		m_memOptGroup.EnableWindow(TRUE);
		m_isReduceProcessMemoryControl.EnableWindow(TRUE);
		m_isReduceAllProcessesMemoryControl.EnableWindow(TRUE);
		if(m_isReduceProcessMemory)
		{
			m_delayOptimHiddenAppControl.EnableWindow(TRUE);
			m_delayOptimHiddenAppTextControl.EnableWindow(TRUE);
		}
		else
		{
			m_delayOptimHiddenAppControl.EnableWindow(FALSE);
			m_delayOptimHiddenAppTextControl.EnableWindow(FALSE);
		}
		//
		if(m_isReduceAllProcessesMemory)
		{
			m_delayOptimAllAppControl.EnableWindow(TRUE);
			m_delayOptimAllAppTextControl.EnableWindow(TRUE);
		}
		else
		{
			m_delayOptimAllAppControl.EnableWindow(FALSE);
			m_delayOptimAllAppTextControl.EnableWindow(FALSE);
		}
	} 
	//
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


void CPPagePerfomance::PrepareVariables()
{
	m_delayOptimHiddenApp_index	= m_delayOptimHiddenAppControl.GetCurSel();
	m_delayOptimHiddenApp = m_delayA[m_delayOptimHiddenApp_index];
	//
	m_delayOptimAllApp_index = m_delayOptimAllAppControl.GetCurSel();
	m_delayOptimAllApp = m_delayA[m_delayOptimAllApp_index];
}


void CPPagePerfomance::OnSelchangeDelaycombohidden() 
{
	UpdateData(TRUE);
	SetModified(TRUE);	
}

void CPPagePerfomance::OnSelchangeDelaycombohiddenall() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPagePerfomance::OnChangepriority() 
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

void CPPagePerfomance::OnProcesspriority() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPagePerfomance::OnPriorityNormal() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPagePerfomance::OnPriorityLow() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}

void CPPagePerfomance::OnChangeprioritypm() 
{
	UpdateData(TRUE);
	SetModified(TRUE);
}
