#include "StdAfx.h"
#include "WndStore.h"
#include "Hide.h"
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ERROR_TITLE _T("Error")

IMPLEMENT_SERIAL(CWndInfoStore, CObject, VERSIONABLE_SCHEMA | 2 )

CWndInfoStore::CWndInfoStore()
{
	m_wndName = _T("X?X?X?X?X?X::XX::XX::?_!@#S");
	m_progName = _T("X?X?X?X?X?X::XX::XX::?_!@#S");
	m_isHideWhenMinimize = 0;
	m_isHideWhenCreated = 0;
	m_isHideByRMouse = 0;
	m_isHideByDblRMouse = 0;
	m_hideMethod = 0;
	m_isShowOnActivateApp = 0;
	m_hideTo = 0;
	m_isActivateNextWindow = 0;
	m_wHash = 0;
	m_isPwdUseShowWindow = 0;
	m_isChangeProcessPriority = 0;
	m_processPriority = 0;
	m_isDontChangeProcessPriorityPM = 0;
	m_isReduceProcessMemory = 0;
}

CWndInfoStore::~CWndInfoStore()
{

}

void CWndInfoStore::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_wndName;
		ar << m_progName;
		ar << m_isHideWhenMinimize;
		ar << m_isHideWhenCreated;
		ar << m_isHideByRMouse;
		ar << m_isHideByDblRMouse;
		ar << m_hideMethod;
		ar << m_isShowOnActivateApp;
		ar << m_hideTo;
		ar << m_isActivateNextWindow;
		ar << m_wHash;
		ar << m_isPwdUseShowWindow;
		ar << m_isChangeProcessPriority;
		ar << m_processPriority;
		ar << m_isDontChangeProcessPriorityPM;
		ar << m_isReduceProcessMemory;
	}
	else
	{
		ar >> m_wndName;
		ar >> m_progName;
		ar >> m_isHideWhenMinimize;
		ar >> m_isHideWhenCreated;
		ar >> m_isHideByRMouse;
		ar >> m_isHideByDblRMouse;
		ar >> m_hideMethod;
		ar >> m_isShowOnActivateApp;
		ar >> m_hideTo;
		ar >> m_isActivateNextWindow;
		ar >> m_wHash;
		ar >> m_isPwdUseShowWindow;
		ar >> m_isChangeProcessPriority;
		ar >> m_processPriority;
		ar >> m_isDontChangeProcessPriorityPM;
		ar >> m_isReduceProcessMemory;
	};
}
//
CWndStore::CWndStore()
{
	m_isOpen = FALSE;
}

CWndStore::~CWndStore()
{
	TRY
	{
		if(m_isOpen)
			Close();
	}
	CATCH( CFileException, e )
	{
		#ifdef _DEBUG
			afxDump << "File " << m_lpszFileName << " cannot be closed "<< e->m_cause << "\n";
		#endif
	}
	END_CATCH
}

BOOL CWndStore::ReadInfo(LPCTSTR lpszFileName)
{
	BOOL bResult = FALSE;
	CHideApp* app = (CHideApp*) AfxGetApp();
	CFileException ex;
	UINT openFlags = CFile::modeNoTruncate|CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive;

	m_lpszFileName = lpszFileName;
	
	int k = app->m_wndInfoStoreArray.GetSize();
	for(int i = 0; i < k; i++)
	{
		CWndInfoStore* wndInfo = (CWndInfoStore*) app->m_wndInfoStoreArray.GetAt(0);
		app->m_wndInfoStoreArray.RemoveAt(0);
		delete wndInfo;
	}
	app->m_wndInfoStoreArray.RemoveAll();
	if(!CFile::Open(lpszFileName, openFlags, &ex))
	{
		TCHAR szCause[255];
		ex.GetErrorMessage(szCause, 255, NULL);
		MessageBoxEx(NULL , _T(szCause), ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
	} else
		m_isOpen = TRUE;

	CArchive ar(this, CArchive::load);
	TRY
	{
		app->m_wndInfoStoreArray.Serialize(ar);
		app->m_wndInfoStoreImageList.DeleteImageList();
		app->m_wndInfoStoreImageList.Read(&ar);
		bResult = TRUE;
	}
	CATCH( CArchiveException, e )
	{
		app->m_wndInfoStoreArray.RemoveAll();
		app->m_wndInfoStoreArray.SetSize(0);
		app->m_wndInfoStoreImageList.DeleteImageList();
		app->m_wndInfoStoreImageList.Create(16, 16, ILC_COLOR4|ILC_MASK, 8, 4);
	}
	END_CATCH
	//
	ar.Close();
	Close();
	m_isOpen = FALSE;
	return bResult;
}

BOOL CWndStore::WriteInfo(LPCTSTR lpszFileName)
{
	BOOL bResult = FALSE;
	CHideApp* app = (CHideApp*) AfxGetApp();	
	CFileException ex;
	UINT openFlags = CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive;

	m_lpszFileName = lpszFileName;
	if(!CFile::Open(lpszFileName, openFlags, &ex))
	{
		TCHAR szCause[255];
		ex.GetErrorMessage(szCause, 255, NULL);
		MessageBoxEx(NULL , _T(szCause), ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
	} else
		m_isOpen = TRUE;

	CArchive ar(this, CArchive::store);
	TRY
	{
		if(app->m_wndInfoStoreArray.GetSize() > 0)
			app->m_wndInfoStoreArray.Serialize(ar);
		if(app->m_wndInfoStoreImageList.m_hImageList != NULL)
			app->m_wndInfoStoreImageList.Write(&ar);
		bResult = TRUE;
	}
	CATCH( CArchiveException, e )
	{
		TCHAR szCause[255];
		e->GetErrorMessage(szCause, 255, NULL);
		MessageBoxEx(NULL , _T(szCause), ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
	}
	END_CATCH
	ar.Close();
	Flush();
	Close();
	m_isOpen = FALSE;
	return bResult;
}
