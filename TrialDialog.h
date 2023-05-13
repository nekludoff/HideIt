#if !defined(AFX_TRIALDIALOG_H__B5D1C853_211F_11D6_AA97_00D0B75DC76B__INCLUDED_)
#define AFX_TRIALDIALOG_H__B5D1C853_211F_11D6_AA97_00D0B75DC76B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrialDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrialDialog dialog

class CTrialDialog : public CDialog
{
// Construction
public:
	CTrialDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTrialDialog)
	enum { IDD = IDD_TRIAL_DIALOG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrialDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrialDialog)
	afx_msg void OnBuynow();
	afx_msg void OnRegButton();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRIALDIALOG_H__B5D1C853_211F_11D6_AA97_00D0B75DC76B__INCLUDED_)
