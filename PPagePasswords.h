#if !defined(AFX_PPAGEPASSWORDS_H__89D7C1AF_E282_4711_BBF3_F88665699138__INCLUDED_)
#define AFX_PPAGEPASSWORDS_H__89D7C1AF_E282_4711_BBF3_F88665699138__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPagePasswords.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPagePasswords dialog

class CPPagePasswords : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPagePasswords)

// Construction
public:
	CPPagePasswords();
	~CPPagePasswords();

// Dialog Data
	//{{AFX_DATA(CPPagePasswords)
	enum { IDD = IDD_PPAGEPASSWORDS };
	CButton	m_isPwdUseShowWindowControl;
	CEdit	m_masterPasswordControl;
	CEdit	m_confirmMasterPasswordControl;
	CButton	m_masterBox;
	CButton	m_groupBox;
	CString	m_masterPassword;
	CString	m_confirmMasterPassword;
	BOOL	m_isPwdUseExit;
	BOOL	m_isPwdUseSettings;
	BOOL	m_isPwdUseShowMenu;
	BOOL	m_isPwdUseShowPrivateMenu;
	BOOL	m_isPwdUseShowWindow;
	CString	m_oldMasterPassword;
	BOOL	m_isPwdUseWndSettings;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPagePasswords)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPagePasswords)
	afx_msg void OnChangeMasterpwd();
	afx_msg void OnChangeMasterpwdconfirm();
	afx_msg void OnPwduseexit();
	afx_msg void OnPwdusesettings();
	afx_msg void OnPwduseshowmenu();
	afx_msg void OnPwduseshowprivatemenu();
	afx_msg void OnPwduseshowwindow();
	afx_msg void OnKillfocusMasterpwdconfirm();
	virtual BOOL OnInitDialog();
	afx_msg void OnPwdusewndsettings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPAGEPASSWORDS_H__89D7C1AF_E282_4711_BBF3_F88665699138__INCLUDED_)
