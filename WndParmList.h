#if !defined(AFX_WNDPARMLIST_H__061A2A4C_77CE_4AAC_95D0_EA4F5DA77739__INCLUDED_)
#define AFX_WNDPARMLIST_H__061A2A4C_77CE_4AAC_95D0_EA4F5DA77739__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndParmList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWndParmList dialog

class CWndParmList : public CDialog
{
// Construction
public:
	CWndParmList(CWnd* pParent = NULL);   // standard constructor
	CImageList *m_pImageList;
	void PrepareItems(void);
// Dialog Data
	//{{AFX_DATA(CWndParmList)
	enum { IDD = IDD_WNDPARMLIST };
	CListCtrl	m_wndList;
	int			m_listIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndParmList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWndParmList)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkWndlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnWndlstedit();
	afx_msg void OnWndlstdelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDPARMLIST_H__061A2A4C_77CE_4AAC_95D0_EA4F5DA77739__INCLUDED_)
