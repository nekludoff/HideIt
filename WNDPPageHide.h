#if !defined(AFX_WNDPAGEHIDE_H__BCDE8DE0_31B4_40AE_93B7_5DBF1F006E73__INCLUDED_)
#define AFX_WNDPPAGEHIDE_H__BCDE8DE0_31B4_40AE_93B7_5DBF1F006E73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WNDPPageHide.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWNDPPageHide dialog

class CWNDPPageHide : public CPropertyPage
{
	DECLARE_DYNCREATE(CWNDPPageHide)

// Construction
public:
	CWNDPPageHide();
	~CWNDPPageHide();

// Dialog Data
	//{{AFX_DATA(CWNDPPageHide)
	enum { IDD = IDD_WNDPPAGEHIDE };
	CButton	m_isShowOnActivateAppControl;
	CButton	m_isHideWhenCreatedControl;
	BOOL	m_isHideWhenMinimize;
	BOOL	m_isHideByRMouse;
	BOOL	m_isShowOnActivateApp;
	int		m_hideTo;
	int		m_hideMethod;
	BOOL	m_isActivateNextWindow;
	BOOL	m_isHideByDblRMouse;
	BOOL	m_isHideWhenCreated;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWNDPPageHide)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWNDPPageHide)
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
	afx_msg void OnHideCreate();
	afx_msg void OnRadiohidetoPrivatemenucommon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDPPAGEHIDE_H__BCDE8DE0_31B4_40AE_93B7_5DBF1F006E73__INCLUDED_)
