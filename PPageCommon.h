#if !defined(AFX_PPAGECOMMON_H__9D19E052_1627_11D6_AF48_00D0B75DC76B__INCLUDED_)
#define AFX_PPAGECOMMON_H__9D19E052_1627_11D6_AF48_00D0B75DC76B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPageCommon.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPageCommon dialog

class CPPageCommon : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPageCommon)

// Construction
public:
	CPPageCommon();
	~CPPageCommon();

// Dialog Data
	//{{AFX_DATA(CPPageCommon)
	enum { IDD = IDD_PPAGECOMMON };
	BOOL	m_isProcessInvisibleWindows;
	BOOL	m_isSplashWindow;
	BOOL	m_isAddHideAll;
	BOOL	m_isAutoStart;
	BOOL	m_isSoftExit;
	BOOL	m_isAddHideIt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPageCommon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPageCommon)
	afx_msg void OnProcessinvisiblewnd();
	afx_msg void OnSplashwindow();
	afx_msg void OnAddhideall();
	afx_msg void OnAutostart();
	afx_msg void OnSoftexit();
	afx_msg void OnAddhide();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPAGECOMMON_H__9D19E052_1627_11D6_AF48_00D0B75DC76B__INCLUDED_)
