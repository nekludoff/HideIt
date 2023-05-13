#if !defined(AFX_PPAGEPERFOMANCE_H__9D19E053_1627_11D6_AF48_00D0B75DC76B__INCLUDED_)
#define AFX_PPAGEPERFOMANCE_H__9D19E053_1627_11D6_AF48_00D0B75DC76B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPagePerfomance.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPagePerfomance dialog

class CPPagePerfomance : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPagePerfomance)

// Construction
public:
	UINT m_delayOptimAllApp;
	UINT m_delayOptimAllApp_index;
	UINT m_delayOptimHiddenApp_index;
	UINT m_delayOptimHiddenApp;
	CPPagePerfomance();
	~CPPagePerfomance();

// Dialog Data
	//{{AFX_DATA(CPPagePerfomance)
	enum { IDD = IDD_PPAGEPERFOMANCE };
	CButton	m_isDontChangeProcessPriorityPMControl;
	CButton	m_priorityGroup;
	CButton	m_memOptGroup;
	CButton	m_isReduceProcessMemoryControl;
	CButton	m_priorityNormalControl;
	CButton	m_priorityLowControl;
	CButton	m_processPriorityControl;
	CButton	m_isChangeProcessPriorityControl;
	CStatic	m_delayOptimHiddenAppTextControl;
	CStatic	m_delayOptimAllAppTextControl;
	CButton	m_isReduceAllProcessesMemoryControl;
	CComboBox	m_delayOptimAllAppControl;
	CComboBox	m_delayOptimHiddenAppControl;
	BOOL	m_isReduceProcessMemory;
	BOOL	m_isReduceAllProcessesMemory;
	BOOL	m_isChangeProcessPriority;
	int		m_processPriority;
	BOOL	m_isDontChangeProcessPriorityPM;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPagePerfomance)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPagePerfomance)
	afx_msg void OnReducememory();
	virtual BOOL OnInitDialog();
	afx_msg void OnReduceallmemory();
	afx_msg void OnSelchangeDelaycombohidden();
	afx_msg void OnSelchangeDelaycombohiddenall();
	afx_msg void OnChangepriority();
	afx_msg void OnProcesspriority();
	afx_msg void OnPriorityNormal();
	afx_msg void OnPriorityLow();
	afx_msg void OnChangeprioritypm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	UINT m_delayA[11];
	void PrepareVariables(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPAGEPERFOMANCE_H__9D19E053_1627_11D6_AF48_00D0B75DC76B__INCLUDED_)
