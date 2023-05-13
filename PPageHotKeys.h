#if !defined(AFX_PPAGEHOTKEYS_H__5DA83963_3727_11D6_AAB8_00D0B75DC76B__INCLUDED_)
#define AFX_PPAGEHOTKEYS_H__5DA83963_3727_11D6_AAB8_00D0B75DC76B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPageHotKeys.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CPPageHotKeys dialog

class CPPageHotKeys : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPageHotKeys)

// Construction
public:
	void SetHotKeyText();
	CString GetHotKeyName(WORD wVirtualKeyCode, WORD wModifiers);
	int m_hotKeyArray[10][3];
	CPPageHotKeys();
	~CPPageHotKeys();

// Dialog Data
	//{{AFX_DATA(CPPageHotKeys)
	enum { IDD = IDD_PPAGEHOTKEYS };
	CListBox	m_actionListHKControl;
	CHotKeyCtrl	m_hotKeyControl;
	CListBox	m_actionListControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPageHotKeys)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPageHotKeys)
	afx_msg void OnSelchangeActionlist();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_lastIndex;

};

void PASCAL DDV_HotKey(CDataExchange* pDX, CHotKeyCtrl* hotKeyControl, CPPageHotKeys* page);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPAGEHOTKEYS_H__5DA83963_3727_11D6_AAB8_00D0B75DC76B__INCLUDED_)
