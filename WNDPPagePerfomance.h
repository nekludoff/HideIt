#if !defined(AFX_WNDPPAGEPERFOMANCE_H__DF56481A_1B99_49A1_AE9A_4FC4B5B68332__INCLUDED_)
#define AFX_WNDPPAGEPERFOMANCE_H__DF56481A_1B99_49A1_AE9A_4FC4B5B68332__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WNDPPagePerfomance.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWNDPPagePerfomance dialog

class CWNDPPagePerfomance : public CPropertyPage
{
	DECLARE_DYNCREATE(CWNDPPagePerfomance)

// Construction
public:
	CWNDPPagePerfomance();
	~CWNDPPagePerfomance();

// Dialog Data
	//{{AFX_DATA(CWNDPPagePerfomance)
	enum { IDD = IDD_WNDPPAGEPERFOMANCE };
	CButton	m_isDontChangeProcessPriorityPMControl;
	CButton	m_priorityGroup;
	CButton	m_isReduceProcessMemoryControl;
	CButton	m_priorityNormalControl;
	CButton	m_priorityLowControl;
	CButton	m_processPriorityControl;
	CButton	m_isChangeProcessPriorityControl;
	BOOL	m_isChangeProcessPriority;
	int		m_processPriority;
	BOOL	m_isDontChangeProcessPriorityPM;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWNDPPagePerfomance)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWNDPPagePerfomance)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangepriority();
	afx_msg void OnProcesspriority();
	afx_msg void OnPriorityNormal();
	afx_msg void OnPriorityLow();
	afx_msg void OnChangeprioritypm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDPPAGEPERFOMANCE_H__DF56481A_1B99_49A1_AE9A_4FC4B5B68332__INCLUDED_)
