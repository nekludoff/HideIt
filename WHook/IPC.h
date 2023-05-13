#ifndef _IPC_H_
#define _IPC_H_

#define IPC_SHARED_MMF		_T("{34F673E0-878F-11D5-B98A-00B0D07B8C7C}")
#define IPC_MUTEX			_T("{34F673E1-878F-11D5-B98A-00B0D07B8C7C}")

// Class for Inter Process Communication using Memory Mapped Files
class CIPC
{
public:
	CIPC();
	virtual ~CIPC();

	bool CreateIPCMMF(void);
	bool OpenIPCMMF(void);
	void CloseIPCMMF(void);

	bool IsOpen(void) const {return (m_hFileMap != NULL);}

	bool ReadIPCMMF(LPBYTE pBuf, DWORD &dwBufSize);
	bool WriteIPCMMF(const LPBYTE pBuf, const DWORD dwBufSize);

	bool Lock(void);
	void Unlock(void);

protected:
	HANDLE m_hFileMap;
	HANDLE m_hMutex;
};

#endif
