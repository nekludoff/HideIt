#include "stdafx.h"
#include "OSInfo.h"

//***********************************************
COSInfo::COSInfo()
{
	ZeroMemory(&m_osvi, sizeof(OSVERSIONINFO));
	m_osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if(!GetVersionEx(&m_osvi))
		ZeroMemory(&m_osvi, sizeof(OSVERSIONINFO));
}

//***********************************************
COSInfo::~COSInfo()
{
}

//***********************************************
EOSType COSInfo::GetOSType(void) const
{
	EOSType eOSType = eUnknown;

	if(IsNT())
	{
		if(IsWindowsNT())
			eOSType = eWinNT;
		else if(IsWindows2K())
			eOSType = eWin2K;
		else if(IsWindowsXP())
			eOSType = eWinXP;
	}
	else
	{
		if(IsWindows95())
			eOSType = eWin95;
		else if(IsWindows98())
			eOSType = eWin98;
		else if(IsWindowsME())
			eOSType = eWinME;
	}

	return eOSType;
}

//***********************************************
bool COSInfo::IsWindows95(void) const
{
	// Windows95 if:
	// Major == 4 and Minor == 0 and PlatformId != NT
	return (m_osvi.dwMajorVersion == 4 &&
		m_osvi.dwMinorVersion == 0 &&
		m_osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) ? true : false;
}

//***********************************************
bool COSInfo::IsWindows98(void) const
{
	// Windows98 if:
	// Major >= 4 and Minor > 0 and PlatformId != NT
	// (except Major == 4 and Minor == 90 which is ME)
	// (note:  Major == 4 and Minor == 0 is 95)
	return (m_osvi.dwMajorVersion >= 4 &&
		m_osvi.dwMinorVersion > 0 &&
		m_osvi.dwPlatformId != VER_PLATFORM_WIN32_NT &&
		!(m_osvi.dwMajorVersion == 4 &&	m_osvi.dwMinorVersion == 90)) ? true : false;
}

//***********************************************
bool COSInfo::IsWindowsME(void) const
{
	// WindowsME if:
	// Major == 4 and Minor == 90 and PlatformId != NT
	return (m_osvi.dwMajorVersion == 4 &&
		m_osvi.dwMinorVersion == 90 &&
		m_osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) ? true : false;
}

//***********************************************
bool COSInfo::IsWindowsNT(void) const
{
	// WindowsNT4 if:
	// Major == 4 and Minor == 0 and PlatformId == NT
	return (m_osvi.dwMajorVersion == 4 &&
		m_osvi.dwMinorVersion == 0 &&
		m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) ? true : false;
}

//***********************************************
bool COSInfo::IsWindows2K(void) const
{
	// Windows2000 if:
	// Major == 5 and Minor == 0 and PlatformId == NT
	return (m_osvi.dwMajorVersion == 5 &&
		m_osvi.dwMinorVersion == 0 &&
		m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) ? true : false;
}

//***********************************************
bool COSInfo::IsWindowsXP(void) const
{
	// WindowsXP if:
	// Major == 5 and Minor == 1 and PlatformId == NT
	// !!! These numbers from a Beta of XP, double-check on final release.
	return (m_osvi.dwMajorVersion == 5 &&
		m_osvi.dwMinorVersion == 1 &&
		m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) ? true : false;
}

//***********************************************
bool COSInfo::IsNT(void) const
{
	return (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) ? true : false;
}

//***********************************************
DWORD COSInfo::GetMajor(void) const
{
	return m_osvi.dwMajorVersion;
}

//***********************************************
DWORD COSInfo::GetMinor(void) const
{
	return m_osvi.dwMinorVersion;
}

//***********************************************
DWORD COSInfo::GetBuild(void) const
{
	return m_osvi.dwBuildNumber;
}

//***********************************************
DWORD COSInfo::GetPlatformId(void) const
{
	return m_osvi.dwPlatformId;
}

//***********************************************
LPCTSTR COSInfo::GetCSDString(void) const
{
	return m_osvi.szCSDVersion;
}
