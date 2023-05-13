#if !defined(AFX_SPLASHWINDOW_H__399CF563_16E9_11D6_AF4B_00D0B75DC76B__INCLUDED_)
#define AFX_SPLASHWINDOW_H__399CF563_16E9_11D6_AF4B_00D0B75DC76B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplashWindow dialog

class CSplashWindow : public CDialog
{
// Construction
public:
	CSplashWindow(CWnd* pParent = NULL);   // standard constructor
	void GSleep(LPCTSTR lpName, UINT uMillisec);
	void SetSplashTimerUp(UINT msec);


// Dialog Data
	//{{AFX_DATA(CSplashWindow)
	enum { IDD = IDD_SPLASHWINDOW };
	CStatic	m_splashIcon;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashWindow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSplashWindow)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASHWINDOW_H__399CF563_16E9_11D6_AF4B_00D0B75DC76B__INCLUDED_)
