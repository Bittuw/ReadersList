#pragma once
#include <Windows.h>
#include <string>
#include "ZBase.h"
#include "ZReader.h"

#define GetBit(val, bitN)	(BOOL)((val >> (bitN)) & 1)
#define GetTickSpan(old, curr)	(DWORD)((curr >= old) ? (curr - old) : (MAXDWORD - old + curr))

extern LPCTSTR PortTypeStrs[5];
extern LPCTSTR ReaderTypeStrs[9];
extern LPCTSTR CardTypeStrs[18];

BOOL CheckZRError(HRESULT nStatus, LPCTSTR pszContext);
//std::wstring ZKeyNumToStr(const Z_KEYNUM& rNum, ZR_CARD_TYPE nType);

class CZReaderLoader
{
public:
	HRESULT m_nStatus;
	
public:
	CZReaderLoader();
	virtual ~CZReaderLoader();

	inline BOOL IsLoaded() const { return SUCCEEDED(m_nStatus); }
};
