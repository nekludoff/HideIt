#include <windows.h>
#include <tchar.h>
#include <crtdbg.h>

#include "IPC.h"

//***********************************************
CIPC::CIPC() : m_hFileMap(NULL), m_hMutex(NULL)
{
}

//***********************************************
CIPC::~CIPC()
{
	CloseIPCMMF();
	Unlock();
}

//***********************************************
bool CIPC::CreateIPCMMF(void)
{
	bool bCreated = false;

	try
	{
		if(m_hFileMap != NULL)
			return false;	// Already created

		// Create an in-memory 4KB memory mapped file to share data
		m_hFileMap = CreateFileMapping((HANDLE)0xFFFFFFFF,
			NULL,
			PAGE_READWRITE,
			0,
			4096,
			IPC_SHARED_MMF);
		if(m_hFileMap != NULL)
			bCreated = true;
	}
	catch(...) {}

	return bCreated;
}

//***********************************************
bool CIPC::OpenIPCMMF(void)
{
	bool bOpened = false;

	try
	{
		if(m_hFileMap != NULL)
			return true;	// Already opened

		m_hFileMap = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE,
			FALSE,
			IPC_SHARED_MMF);
		if(m_hFileMap != NULL)
			bOpened = true;
	}
	catch(...) {}

	return bOpened;
}

//***********************************************
void CIPC::CloseIPCMMF(void)
{
	try
	{
		if(m_hFileMap != NULL)
			CloseHandle(m_hFileMap), m_hFileMap = NULL;
	}
	catch(...) {}
}

//***********************************************
bool CIPC::ReadIPCMMF(LPBYTE pBuf, DWORD &dwBufSize)
{
	_ASSERTE(pBuf);

	bool bSuccess = true;

	try
	{
		if(m_hFileMap == NULL)
			return false;

		DWORD dwBaseMMF = (DWORD)MapViewOfFile(m_hFileMap,
			FILE_MAP_READ | FILE_MAP_WRITE,
			0, 0, 0);
		_ASSERTE(dwBaseMMF);

		// The first DWORD in the MMF contains the size of the data
		DWORD dwSizeofInBuf = dwBufSize;
		CopyMemory(&dwBufSize, (LPVOID)dwBaseMMF, sizeof(DWORD));

		if(dwSizeofInBuf != 0)
		{
			if(dwBufSize > dwSizeofInBuf)
				bSuccess = false;
			else
				CopyMemory(pBuf, (LPVOID)(dwBaseMMF + sizeof(DWORD)), dwBufSize);
		}

		UnmapViewOfFile((LPVOID)dwBaseMMF);
	}
	catch(...) {}

	return bSuccess;
}

//***********************************************
bool CIPC::WriteIPCMMF(const LPBYTE pBuf, const DWORD dwBufSize)
{
	_ASSERTE(pBuf);

	bool bSuccess = true;

	try
	{
		if(m_hFileMap == NULL)
			return false;

		DWORD dwBaseMMF = (DWORD)MapViewOfFile(m_hFileMap,
			FILE_MAP_READ | FILE_MAP_WRITE,
			0, 0, 0);
		_ASSERTE(dwBaseMMF);

		// The first DWORD in the MMF contains the size of the data
		CopyMemory((LPVOID)dwBaseMMF, &dwBufSize, sizeof(DWORD));
		CopyMemory((LPVOID)(dwBaseMMF + sizeof(DWORD)), pBuf, dwBufSize);

		UnmapViewOfFile((LPVOID)dwBaseMMF);
	}
	catch(...) {}

	return bSuccess;
}

//***********************************************
bool CIPC::Lock(void)
{
	bool bLocked = false;

	try
	{
		// First get the handle to the mutex
		m_hMutex = CreateMutex(NULL, FALSE, IPC_MUTEX);
		if(m_hMutex != NULL)
		{
			// Wait to get the lock on the mutex
			if(WaitForSingleObject(m_hMutex, 5000) == WAIT_OBJECT_0)
				bLocked = true;
		}
	}
	catch(...) {}

	return bLocked;
}

//***********************************************
void CIPC::Unlock(void)
{
	try
	{
		if(m_hMutex != NULL)
		{
			ReleaseMutex(m_hMutex);
			CloseHandle(m_hMutex);
			m_hMutex = NULL;
		}
	}
	catch(...) {}
}
