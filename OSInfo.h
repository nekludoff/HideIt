#ifndef _OSINFO_H_
#define _OSINFO_H_

typedef enum _EOSType
{
	eUnknown = 0,
	eWin95,
	eWin98,
	eWinME,
	eWinNT,
	eWin2K,
	eWinXP
} EOSType, *LPEOSType;


class COSInfo
{
public:
	COSInfo();
	virtual ~COSInfo();

	EOSType GetOSType(void) const;

	bool IsWindows95(void) const;
	bool IsWindows98(void) const;
	bool IsWindowsME(void) const;
	bool IsWindowsNT(void) const;
	bool IsWindows2K(void) const;
	bool IsWindowsXP(void) const;

	bool IsNT(void) const;

	DWORD GetMajor(void) const;
	DWORD GetMinor(void) const;
	DWORD GetBuild(void) const;
	DWORD GetPlatformId(void) const;
	LPCTSTR GetCSDString(void) const;

protected:
	OSVERSIONINFO m_osvi;
};

#endif
