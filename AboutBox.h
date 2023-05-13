#if !defined(AFX_ABOUTBOX_H__9D19E055_1627_11D6_AF48_00D0B75DC76B__INCLUDED_)
#define AFX_ABOUTBOX_H__9D19E055_1627_11D6_AF48_00D0B75DC76B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutBox.h : header file
//
#include "HyperLink.h"
/////////////////////////////////////////////////////////////////////////////
// CAboutBox dialog

class CAboutBox : public CDialog
{
// Construction
public:
	DECLARE_DYNCREATE(CAboutBox)
	CAboutBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutBox)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_regInfoControl;
	CStatic	m_clientInfoControl;
	CButton	m_registerControl;
	CHyperLink m_internetControl;
	CHyperLink m_eMailControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnRegister();
	afx_msg void OnReginfo();
	afx_msg void OnInternet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HCURSOR m_hStaticCursor;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTBOX_H__9D19E055_1627_11D6_AF48_00D0B75DC76B__INCLUDED_)
