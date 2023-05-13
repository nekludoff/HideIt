// PPageHotKeys.cpp : implementation file
//

#include "stdafx.h"
#include "hide.h"
#include "PPageHotKeys.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPageHotKeys property page

IMPLEMENT_DYNCREATE(CPPageHotKeys, CPropertyPage)

CPPageHotKeys::CPPageHotKeys() : CPropertyPage(CPPageHotKeys::IDD)
{
	//{{AFX_DATA_INIT(CPPageHotKeys)
	//}}AFX_DATA_INIT
	m_lastIndex = 0;
	for(int i = 0; i < 10; i++)
	{
		m_hotKeyArray[i][0] = 0;
		m_hotKeyArray[i][1] = 0;
		m_hotKeyArray[i][2] = 0;
	}
}

CPPageHotKeys::~CPPageHotKeys()
{
}

void CPPageHotKeys::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPageHotKeys)
	DDX_Control(pDX, IDC_ACTIONLISTHK, m_actionListHKControl);
	DDX_Control(pDX, IDC_HOTKEYMAIN, m_hotKeyControl);
	DDX_Control(pDX, IDC_ACTIONLIST, m_actionListControl);
	//}}AFX_DATA_MAP
	//DDV_HotKey(pDX, &m_hotKeyControl, this);
}


BEGIN_MESSAGE_MAP(CPPageHotKeys, CPropertyPage)
	//{{AFX_MSG_MAP(CPPageHotKeys)
	ON_LBN_SELCHANGE(IDC_ACTIONLIST, OnSelchangeActionlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPageHotKeys message handlers

BOOL CPPageHotKeys::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CHideApp* app = (CHideApp*) AfxGetApp();
	
	for(int i = 0; i < 10; i ++)
	{
		if(!app->m_hotKeyActionNames[i].IsEmpty())
			m_actionListControl.AddString(app->m_hotKeyActionNames[i]);
	}		
	m_actionListControl.SetCurSel(0);
	m_hotKeyControl.SetHotKey(m_hotKeyArray[0][0], m_hotKeyArray[0][1]);

	m_hotKeyControl.SetRules(HKCOMB_NONE, HKCOMB_A |HKCOMB_C|HKCOMB_CA|HKCOMB_S|HKCOMB_SA|HKCOMB_SC|HKCOMB_SCA);
	SetHotKeyText();
	m_actionListHKControl.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPPageHotKeys::OnSelchangeActionlist() 
{
	WORD wVirtualKeyCode, wModifiers;
	DWORD wKeyAndShift = m_hotKeyControl.GetHotKey();
	CMainFrame* mainWnd = (CMainFrame*) AfxGetMainWnd();
	BOOL isDifferent = FALSE;
	UINT wWinModifiers;

	UpdateData(TRUE);
	int nIndex = m_actionListControl.GetCurSel();
	m_actionListHKControl.SetCurSel(nIndex);
	m_hotKeyControl.GetHotKey(wVirtualKeyCode, wModifiers);
	
	if(nIndex != m_lastIndex)
		m_hotKeyControl.SetHotKey(m_hotKeyArray[nIndex][0], m_hotKeyArray[nIndex][1]);

	if(m_lastIndex != nIndex)
	{
		if( (m_hotKeyArray[m_lastIndex][0] != wVirtualKeyCode) || (m_hotKeyArray[m_lastIndex][1] != wModifiers) )
			isDifferent = TRUE;
		m_hotKeyArray[m_lastIndex][0] = wVirtualKeyCode;
		m_hotKeyArray[m_lastIndex][1] = wModifiers;
	}

	if(isDifferent)
	{
		UnregisterHotKey(mainWnd->GetSafeHwnd(), m_hotKeyArray[m_lastIndex][2]);
		//
		wWinModifiers = 0;
		if( (wModifiers & HOTKEYF_ALT) > 0)
			wWinModifiers = wWinModifiers | MOD_ALT;
		if( (wModifiers & HOTKEYF_CONTROL) > 0)
			wWinModifiers = wWinModifiers | MOD_CONTROL;
		if( (wModifiers & HOTKEYF_SHIFT) > 0)
			wWinModifiers = wWinModifiers | MOD_SHIFT;
		//
		if(m_hotKeyArray[m_lastIndex][0] != 0)
		{
			if(!RegisterHotKey(mainWnd->GetSafeHwnd(), m_hotKeyArray[m_lastIndex][2], wWinModifiers, m_hotKeyArray[m_lastIndex][0]))
			{
				m_hotKeyArray[m_lastIndex][0] = 0;
				m_hotKeyArray[m_lastIndex][1] = 0;
			}
		}
		SetModified(TRUE);
		SetHotKeyText();
	}
	if(m_lastIndex != nIndex)
	{
		m_lastIndex = nIndex;
	}
}


void CPPageHotKeys::OnOK()
{
	WORD wVirtualKeyCode, wModifiers;
	DWORD wKeyAndShift = m_hotKeyControl.GetHotKey();
	CMainFrame* mainWnd = (CMainFrame*) AfxGetMainWnd();
	BOOL isDifferent = FALSE;
	UINT wWinModifiers;

	UpdateData(TRUE);
	int nIndex = m_actionListControl.GetCurSel();
	m_hotKeyControl.GetHotKey(wVirtualKeyCode, wModifiers);

	m_hotKeyControl.SetHotKey(wVirtualKeyCode, wModifiers);

	if( (m_hotKeyArray[nIndex][0] != wVirtualKeyCode) || (m_hotKeyArray[nIndex][1] != wModifiers) )
		isDifferent = TRUE;
	m_hotKeyArray[nIndex][0] = wVirtualKeyCode;
	m_hotKeyArray[nIndex][1] = wModifiers;

	if(isDifferent)
	{
		UnregisterHotKey(mainWnd->GetSafeHwnd(), m_hotKeyArray[nIndex][2]);
		//
		wWinModifiers = 0;
		if( (wModifiers & HOTKEYF_ALT) > 0)
			wWinModifiers = wWinModifiers | MOD_ALT;
		if( (wModifiers & HOTKEYF_CONTROL) > 0)
			wWinModifiers = wWinModifiers | MOD_CONTROL;
		if( (wModifiers & HOTKEYF_SHIFT) > 0)
			wWinModifiers = wWinModifiers | MOD_SHIFT;
		//
		RegisterHotKey(mainWnd->GetSafeHwnd(), m_hotKeyArray[nIndex][2], wWinModifiers, m_hotKeyArray[nIndex][0]);
	}

	CPropertyPage::OnOK();
}

void PASCAL DDV_HotKey(CDataExchange* pDX, CHotKeyCtrl* hotKeyControl, CPPageHotKeys* page)
{
	if(pDX->m_bSaveAndValidate)
	{
		//page->SetModified(TRUE);
	}
}

CString CPPageHotKeys::GetHotKeyName(WORD wVirtualKeyCode, WORD wModifiers)
{
	char aKeyName[21];
	CString str = _T("");

	if(wVirtualKeyCode == 0)
		str = _T("None");
	else
	{
		LONG scanCode = MapVirtualKey(wVirtualKeyCode, 0);
		GetKeyNameText(scanCode * 65536, aKeyName, 20);
		if( (wModifiers & HOTKEYF_CONTROL) > 0)
			str += "Ctrl + ";
		if( (wModifiers & HOTKEYF_SHIFT) > 0)
			str += "Shift + ";
		if( (wModifiers & HOTKEYF_ALT) > 0)
			str += "Alt + ";
		str += aKeyName;
	}
	return str;
}

void CPPageHotKeys::SetHotKeyText()
{
	CHideApp* app = (CHideApp*) AfxGetApp();
	for(int i = 0; i < 10; i ++)
	{
		if(!app->m_hotKeyActionNames[i].IsEmpty())
		{
			CString hotKeyName = GetHotKeyName(m_hotKeyArray[i][0], m_hotKeyArray[i][1]);
			m_actionListHKControl.DeleteString(i);
			m_actionListHKControl.InsertString(i, hotKeyName);
		}
	}		
}

