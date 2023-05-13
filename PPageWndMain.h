#if !defined(AFX_PPAGEWNDMAIN_H__28D8E451_09DE_4B25_8DAF_3E4B04EB87D3__INCLUDED_)
#define AFX_PPAGEWNDMAIN_H__28D8E451_09DE_4B25_8DAF_3E4B04EB87D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPageWndMain.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPageWndMain dialog

class CPPageWndMain : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPageWndMain)

// Construction
public:
	CPPageWndMain();
	~CPPageWndMain();

// Dialog Data
	//{{AFX_DATA(CPPageWndMain)
	enum { IDD = IDD_PPAGEWNDMAIN };
	CStatic	m_wndIcon;
	CString	m_wndExeName;
	CString	m_wndName;
	int	m_wndListIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPageWndMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPageWndMain)
	afx_msg void OnChangeWndname();
	virtual BOOL OnInitDialog();
	afx_msg void OnClearbutton();
	afx_msg void OnRestorebutton();
	afx_msg void OnClearexebutton();
	afx_msg void OnFindbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPAGEWNDMAIN_H__28D8E451_09DE_4B25_8DAF_3E4B04EB87D3__INCLUDED_)
