#include "stdafx.h"
#include "WndDescr.h"

IMPLEMENT_DYNAMIC(CWndDescr, CObject)

CWndDescr::CWndDescr()
{
	m_hWnd = NULL;
	m_hWndParent = NULL;
	m_windowText = "";
	m_programName = "";
	m_bigIcon = NULL;
	m_smallIcon = NULL;
	m_processId = 0;
	m_threadId = 0;
	m_createTime = NULL;
	m_typeHidden = 0;
	m_wndId = 0;
	m_foregroundWindow = NULL;
	m_priorityClass = 0;
	m_isPriorityChanged = FALSE;
	m_processId = 0;
	m_isMaster = 0;
}

CWndDescr::~CWndDescr()
{
}

