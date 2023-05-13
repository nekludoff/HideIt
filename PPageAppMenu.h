#if !defined(AFX_PPAGEAPPMENU_H__399CF562_16E9_11D6_AF4B_00D0B75DC76B__INCLUDED_)
#define AFX_PPAGEAPPMENU_H__399CF562_16E9_11D6_AF4B_00D0B75DC76B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPageAppMenu.h : header file
//

void PASCAL DDV_TrimChars(CDataExchange* pDX, CString str, CEdit* control);
/////////////////////////////////////////////////////////////////////////////
// CPPageAppMenu dialog

class CPPageAppMenu : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPageAppMenu)

// Construction
public:
	CPPageAppMenu();
	~CPPageAppMenu();

// Dialog Data
	//{{AFX_DATA(CPPageAppMenu)
	enum { IDD = IDD_PPAGEAPPMENU };
	CEdit	m_trimChars;
	BOOL	m_isSmallIcons;
	BOOL	m_isTrimCaption;
	CString	m_trimCaptionChars;
	BOOL	m_isAddHideAllApp;
	BOOL	m_isAddShowAllApp;
	BOOL	m_isExtractIconFromFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPageAppMenu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPageAppMenu)
	afx_msg void OnMenuIcontype();
	afx_msg void OnTrimcaption();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusTrimchars();
	afx_msg void OnChangeTrimchars();
	afx_msg void OnAddshowall();
	afx_msg void OnAddhideall();
	afx_msg void OnExtracticonfromexe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPAGEAPPMENU_H__399CF562_16E9_11D6_AF4B_00D0B75DC76B__INCLUDED_)
