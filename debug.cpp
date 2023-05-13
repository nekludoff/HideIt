#include "stdafx.h"
#include "debug.h"
#include "string.h"
#include <tchar.h>

#ifdef _DEBUG
void WriteLog(LPCTSTR fileName, LPCTSTR logMessage)
{
	DWORD dwBytesWritten;
	CHAR szBuf[3] = "\0";
	HANDLE hFile = CreateFile(fileName,
		GENERIC_WRITE,             
		FILE_SHARE_READ || FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
 
	if(hFile != INVALID_HANDLE_VALUE) 
	{ 
		szBuf[0] = 13;
		szBuf[1] = 10;
		SetFilePointer(hFile, 0, 0, FILE_END);
		WriteFile(hFile, logMessage, strlen(logMessage), &dwBytesWritten, NULL);
		WriteFile(hFile, szBuf, 2, &dwBytesWritten, NULL);
			
	} 
	CloseHandle(hFile);
}
#else
void WriteLog(LPCTSTR fileName, LPCTSTR logMessage)
{
 ;
}
#endif
