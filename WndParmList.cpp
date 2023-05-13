// WndParmList.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "WndParmList.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndParmList dialog


CWndParmList::CWndParmList(CWnd* pParent /*=NULL*/)
	: CDialog(CWndParmList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWndParmList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CWndParmList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWndParmList)
	DDX_Control(pDX, IDC_WNDLIST, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWndParmList, CDialog)
	//{{AFX_MSG_MAP(CWndParmList)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_WNDLIST, OnDblclkWndlist)
	ON_BN_CLICKED(IDC_WNDLSTEDIT, OnWndlstedit)
	ON_BN_CLICKED(IDC_WNDLSTDELETE, OnWndlstdelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWndParmList message handlers

BOOL CWndParmList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	PrepareItems();
	this->SetForegroundWindow();
	this->ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWndParmList::PrepareItems(void)
{
	CHideApp* app = (CHideApp*) AfxGetApp();

	m_wndList.InsertColumn(0,"Window Name", LVCFMT_LEFT, 260);
	m_wndList.InsertColumn(1,"Program Name", LVCFMT_LEFT, 768);

	CString strText;
	int nColumnCount = m_wndList.GetHeaderCtrl()->GetItemCount();

	int k = app->m_wndInfoStoreArray.GetSize();
	for(int i = 0; i < k; i++)
	{
		CWndInfoStore* wndInfo = (CWndInfoStore*) app->m_wndInfoStoreArray.GetAt(i);
	    m_wndList.InsertItem(i, wndInfo->m_wndName, i);
		for (int j=1;j < nColumnCount;j++)
		{
			m_wndList.SetItemText(i, j, wndInfo->m_progName);
		}
   	}
	m_wndList.SetImageList(&app->m_wndInfoStoreImageList, LVSIL_SMALL);
	m_listIndex = -1;
}

void CWndParmList::OnDestroy() 
{
	CHideApp* app = (CHideApp*) AfxGetApp();

	CDialog::OnDestroy();

	app->m_wndStore.WriteInfo(app->m_wndInfoStoreFileName);
	app->m_wndStore.ReadInfo(app->m_wndInfoStoreFileName);
}

void CWndParmList::OnDblclkWndlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnWndlstedit();
	*pResult = 0;
}

void CWndParmList::OnWndlstedit() 
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	m_listIndex = m_wndList.GetNextItem(-1, LVNI_SELECTED);
	if(m_listIndex != -1)
	{
		CMainFrame* pFrame = (CMainFrame*) app->m_pMainWnd;
		pFrame->ShowWindowSettings(m_listIndex);
		m_wndList.DeleteAllItems();
		m_wndList.DeleteColumn(1);
		m_wndList.DeleteColumn(0);
		PrepareItems();
	}
}

void CWndParmList::OnWndlstdelete() 
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	m_listIndex = m_wndList.GetNextItem(-1, LVNI_SELECTED);
	if(m_listIndex != -1)
	{
		if(::MessageBox(this->GetSafeHwnd(), "Are you sure to delete this item?","Hide It!", MB_YESNO|MB_ICONWARNING) == IDYES)
		{
			CWndInfoStore* wndStore = (CWndInfoStore*) app->m_wndInfoStoreArray.GetAt(m_listIndex);
			if(wndStore != NULL)
			{
				app->m_wndInfoStoreImageList.Remove(m_listIndex);
				app->m_wndInfoStoreArray.RemoveAt(m_listIndex, 1);
				delete wndStore;
				m_wndList.DeleteAllItems();
				m_wndList.DeleteColumn(1);
				m_wndList.DeleteColumn(0);
				PrepareItems();
			}
		}
	}	
}
