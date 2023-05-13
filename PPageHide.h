#if !defined(AFX_PPageHide_H__9D19E052_1627_11D6_AF48_00D0B75DC76B__INCLUDED_)
#define AFX_PPageHide_H__9D19E052_1627_11D6_AF48_00D0B75DC76B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPageHide.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPageHide dialog

class CPPageHide : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPageHide)

// Construction
public:
	CPPageHide();
	~CPPageHide();
// Dialog Data
	//{{AFX_DATA(CPPageHide)
	enum { IDD = IDD_PPAGEHIDE };
	BOOL	m_isHideWhenMinimize;
	BOOL	m_isHideByRMouse;
	BOOL	m_isShowOnActivateApp;
	int		m_hideTo;
	int		m_hideMethod;
	BOOL	m_isActivateNextWindow;
	BOOL	m_isHideByDblRMouse;
	BOOL	m_isShowPopupWndMenu;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPageHide)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPageHide)
	afx_msg void OnHideMinimize();
	afx_msg void OnMousehide();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowapp();
	afx_msg void OnRadiohideto();
	afx_msg void OnRadiohidetoMenutray();
	afx_msg void OnRadiohidetoTray();
	afx_msg void OnHideallwndapp();
	afx_msg void OnRadiohidemethod();
	afx_msg void OnRadiohidemethod2();
	afx_msg void OnRadiohidemethod3();
	afx_msg void OnActivatenext();
	afx_msg void OnMousehidedbl();
	afx_msg void OnRadiohidetoPrivatemenu();
	afx_msg void OnWndmenu();
	afx_msg void OnRadiohidetoPrivatemenucommon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPageHide_H__9D19E052_1627_11D6_AF48_00D0B75DC76B__INCLUDED_)
