#include "StdAfx.h"
#include "InfoStore.h"
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ERROR_TITLE _T("Error")

CInfoStore::CInfoStore()
{
	m_isOpen = FALSE;
}

int CInfoStore::Create(LPCTSTR lpszFileName)
{
	m_lpszFileName = lpszFileName;
	Open(m_lpszFileName);
	return 1;
}


int CInfoStore::Open(LPCTSTR lpszFileName)
{
	CFileException ex;
	UINT openFlags = CFile::modeNoTruncate|CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive;
	if(!CFile::Open(lpszFileName, openFlags, &ex))
	{
		TCHAR szCause[255];
		ex.GetErrorMessage(szCause, 255, NULL);
		MessageBoxEx(NULL , _T(szCause), ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
	} else
		m_isOpen = TRUE;
	return 1;
}


CInfoStore::~CInfoStore()
{
	TRY
	{
		if(m_isOpen)
			Close();
		CFile::Remove(m_lpszFileName);
	}
	CATCH( CFileException, e )
	{
		#ifdef _DEBUG
			afxDump << "File " << m_lpszFileName << " cannot be removed "<< e->m_cause << "\n";
		#endif
	}
	END_CATCH
}

BOOL CInfoStore::ReadInfo(DWORD dwInfoId, LPINFOSTORE infoStore)
{
	BOOL bResult = FALSE;
	UINT nBytesRead;
	INFOSTORE iStore;
	SeekToBegin();
	if(dwInfoId == -1)
	{
		do
		{
			nBytesRead = Read(&iStore, sizeof(INFOSTORE));
			if(iStore.dwState != 0)
				break;
		} 
		while( nBytesRead > 0 );
	} 
	else
	{
		do
		{
			nBytesRead = Read(&iStore, sizeof(INFOSTORE));
			if(iStore.dwInfoId == dwInfoId)
				break;
		} 
		while( nBytesRead > 0 );
	}
	if(nBytesRead > 0)
	{
		memcpy(infoStore, &iStore, sizeof(INFOSTORE));
		bResult = TRUE;
	}
	return bResult;
}

BOOL CInfoStore::WriteInfo(INFOSTORE infoStore)
{
	BOOL bResult = FALSE;
	UINT nBytesRead;
	INFOSTORE iStore;
	LONG pos;
	SeekToBegin();
	do
	{
		nBytesRead = Read(&iStore, sizeof(INFOSTORE));
		if( (iStore.hWnd == infoStore.hWnd) && 
			(iStore.dwProcessId == infoStore.dwProcessId) && 
			(iStore.dwThreadId == infoStore.dwThreadId)
			)
			break;
	} 
	while( nBytesRead > 0 );
	if(nBytesRead > 0)
		pos = GetPosition() - sizeof(INFOSTORE);
	else
		pos = GetFreePosition();
	Seek(pos, CFile::begin);
	Write(&infoStore, sizeof(INFOSTORE));
	Flush();
	return bResult;
}

LONG CInfoStore::GetFreePosition()
{
	INFOSTORE iStore;
	UINT nBytesRead;
	SeekToBegin();
	do
	{
		nBytesRead = Read(&iStore, sizeof(INFOSTORE));
		if( iStore.dwState == 0 )
			break;

	} 
	while( nBytesRead > 0 );
	LONG pos = GetPosition();
	if( nBytesRead == 0 )
		return pos;
	else
		return pos - sizeof(INFOSTORE);
}
