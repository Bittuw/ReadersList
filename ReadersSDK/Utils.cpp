#include "stdafx.h"
#include "Utils.h"

LPCTSTR PortTypeStrs[] = {
	TEXT("Unknown"), 
	TEXT("COM"), 
	TEXT("FT"), 
	TEXT("IP"), 
	TEXT("IPS")
};
LPCTSTR ReaderTypeStrs[] = {
	TEXT("Unknown"), 
	TEXT("Z-2 USB"), 
	TEXT("Matrix III Rd-All"), 
	TEXT("Z-2 USB MF"), 
	TEXT("Matrix III Net"),
	TEXT("CP-Z-2MF"),
	TEXT("Z-2 EHR"), 
	TEXT("Z-2 Base"), 
	TEXT("Matrix V")
};
LPCTSTR CardTypeStrs[] = {
	TEXT("Unknown"), 
	TEXT("EM"), 
	TEXT("HID"), 
	TEXT("IC"), 
	TEXT("UL"),
	TEXT("1K"), 
	TEXT("4K"), 
	TEXT("DF"), 
	TEXT("PX"),
	TEXT("Cod433 Fix"),
	TEXT("Cod433"),
	TEXT("Dallas"),
	TEXT("CAME"),
	TEXT("Plus"),
	TEXT("Plus 1K"),
	TEXT("Plus 2K"),
	TEXT("Plus 4K"),
	TEXT("Mini")
};

BOOL CheckZRError(HRESULT nStatus, LPCTSTR pszContext)
{
	if (SUCCEEDED(nStatus))
		return TRUE;
	LPTSTR pBuffer = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | ((HRESULT_FACILITY(nStatus) == 4) ? FORMAT_MESSAGE_FROM_HMODULE : FORMAT_MESSAGE_FROM_SYSTEM),
		GetModuleHandle(_T("ZReader.dll")), 
		(DWORD)nStatus,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&pBuffer, 
		0, 
		NULL);
	if (pBuffer != NULL)
	{
		_tprintf(TEXT("%s fail: %s\n"), pszContext, pBuffer);
		LocalFree(pBuffer);
	}
	getchar();
	return FALSE;
}

std::wstring ZKeyNumToStr(const Z_KEYNUM& rNum, ZR_CARD_TYPE nType)
{
	std::wstring s;
	switch (nType)
	{
	case ZR_CD_EM:
		{
			s.resize(32);
			int n = _stprintf_s((LPTSTR)s.data(), s.size(), TEXT("[%.2X%.2X] %.3d,%.5d"), rNum[5], rNum[4], rNum[3], *(PWORD)&rNum[1]);
			s.resize(max(n, 0));
		}
		break;
	case ZR_CD_HID:
		{
			DWORD nFacility = 0;
			int nFCnt = min(rNum[0] - 2, 4);
			memcpy_s(&nFacility, sizeof(nFacility), &rNum[3], nFCnt);
			s.resize(32);
			int n = _stprintf_s((LPTSTR)s.data(), s.size(), TEXT("[%.*X] %d"), nFCnt*2, nFacility, *(PWORD)&rNum[1]);
			s.resize(max(n, 0));
		}
		break;
	default:
		{
			INT i, j;
			s.resize(rNum[0] * 2 + 1);
			j = 0;
			for (i = rNum[0]; i > 0; i--, j += 2)
				_stprintf_s(&s[j], s.size() - j, TEXT("%.2X"), rNum[i]);
			s.resize(rNum[0] * 2);
		}
	}
	return s;
}

CZReaderLoader::CZReaderLoader()
{
	m_nStatus = LoadZReader();
}

CZReaderLoader::~CZReaderLoader()
{
	if (SUCCEEDED(m_nStatus))
		UnloadZReader();
}
