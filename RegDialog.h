#if !defined(AFX_REGDIALOG_H__B5D1C854_211F_11D6_AA97_00D0B75DC76B__INCLUDED_)
#define AFX_REGDIALOG_H__B5D1C854_211F_11D6_AA97_00D0B75DC76B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegDialog dialog

class CRegDialog : public CDialog
{
// Construction
public:
	CRegDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRegDialog)
	enum { IDD = IDD_REG_DIALOG };
	CString	m_regKey;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegDialog)
	afx_msg void OnPaste();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGDIALOG_H__B5D1C854_211F_11D6_AA97_00D0B75DC76B__INCLUDED_)
