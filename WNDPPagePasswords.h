#if !defined(AFX_WNDPAGEPASSWORDS_H__D442F036_9E84_417C_9B29_F7F8372336A1__INCLUDED_)
#define AFX_WNDPPAGEPASSWORDS_H__D442F036_9E84_417C_9B29_F7F8372336A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WNDPPagePasswords.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWNDPPagePasswords dialog

class CWNDPPagePasswords : public CPropertyPage
{
	DECLARE_DYNCREATE(CWNDPPagePasswords)

// Construction
public:
	CWNDPPagePasswords();
	~CWNDPPagePasswords();

// Dialog Data
	//{{AFX_DATA(CWNDPPagePasswords)
	enum { IDD = IDD_WNDPPAGEPASSWORDS };
	CEdit	m_masterPasswordControl;
	CEdit	m_confirmMasterPasswordControl;
	CString	m_masterPassword;
	CString	m_confirmMasterPassword;
	CString	m_oldMasterPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWNDPPagePasswords)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWNDPPagePasswords)
	afx_msg void OnChangeMasterpwd();
	afx_msg void OnChangeMasterpwdconfirm();
	afx_msg void OnKillfocusMasterpwdconfirm();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDPPAGEPASSWORDS_H__D442F036_9E84_417C_9B29_F7F8372336A1__INCLUDED_)
