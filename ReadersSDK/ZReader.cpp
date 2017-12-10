#include "stdafx.h"
#include "ZReader.h"


#ifdef ZREADER_LINKONREQUEST
ZR_GETVERSION ZR_GetVersion = NULL;
ZR_INITIALIZE ZR_Initialize = NULL;
ZR_FINALYZE ZR_Finalyze = NULL;
ZR_UPDATERDFIRMWARE ZR_UpdateRdFirmware = NULL;
ZR_RD_OPEN ZR_Rd_Open = NULL;
ZR_RD_DETTACHPORT ZR_Rd_DettachPort = NULL;
ZR_RD_GETCONNECTIONSTATUS ZR_Rd_GetConnectionStatus = NULL;
ZR_RD_GETWAITSETTINGS ZR_Rd_GetWaitSettings = NULL;
ZR_RD_SETWAITSETTINGS ZR_Rd_SetWaitSettings = NULL;
ZR_RD_SETCAPTURE ZR_Rd_SetCapture = NULL;
ZR_RD_RELEASECAPTURE ZR_Rd_ReleaseCapture = NULL;
ZR_RD_GETINFORMATION ZR_Rd_GetInformation = NULL;
ZR_RD_UPDATEFIRMWARE ZR_Rd_UpdateFirmware = NULL;
ZR_RD_SETNOTIFICATION ZR_Rd_SetNotification = NULL;
ZR_RD_GETNEXTMESSAGE ZR_Rd_GetNextMessage = NULL;
ZR_RD_SEARCHCARDS ZR_Rd_SearchCards = NULL;
ZR_RD_FINDNEXTCARD ZR_Rd_FindNextCard = NULL;
ZR_RD_SELECTCARD ZR_Rd_SelectCard = NULL;
ZR_RD_READULCARD4PAGE ZR_Rd_ReadULCard4Page = NULL;
ZR_RD_WRITEULCARDPAGE ZR_Rd_WriteULCardPage = NULL;
ZR_RD_FINDT57 ZR_Rd_FindT57 = NULL;
ZR_RD_READT57BLOCK ZR_Rd_ReadT57Block = NULL;
ZR_RD_WRITET57BLOCK ZR_Rd_WriteT57Block = NULL;
ZR_RD_RESETT57 ZR_Rd_ResetT57 = NULL;
ZR_DECODET57CONFIG ZR_DecodeT57Config = NULL;
ZR_ENCODET57CONFIG ZR_EncodeT57Config = NULL;
ZR_DECODET57EMMARINE ZR_DecodeT57EmMarine = NULL;
ZR_ENCODET57EMMARINE ZR_EncodeT57EmMarine = NULL;
ZR_DECODET57HID ZR_DecodeT57Hid = NULL;
ZR_ENCODET57HID ZR_EncodeT57Hid = NULL;
ZR_RD_GETENCODEDCARDNUMBER ZR_Rd_GetEncodedCardNumber = NULL;
ZR_RD_AUTHORIZESECT ZR_Rd_AuthorizeSect = NULL;
ZR_RD_AUTHORIZESECTBYEKEY ZR_Rd_AuthorizeSectByEKey = NULL;
ZR_RD_READMFCARDBLOCK ZR_Rd_ReadMfCardBlock = NULL;
ZR_RD_WRITEMFCARDBLOCK ZR_Rd_WriteMfCardBlock = NULL;
ZR_RD_WRITEMFCARDBLOCK4 ZR_Rd_WriteMfCardBlock4 = NULL;
ZR_RD_GETINDICATORSTATE ZR_Rd_GetIndicatorState = NULL;
ZR_RD_SETINDICATORSTATE ZR_Rd_SetIndicatorState = NULL;
ZR_RD_ADDINDICATORFLASH ZR_Rd_AddIndicatorFlash = NULL;
ZR_RD_BREAKINDICATORFLASH ZR_Rd_BreakIndicatorFlash = NULL;
ZR_RD_GETINDICATORFLASHAVAILABLE ZR_Rd_GetIndicatorFlashAvailable = NULL;
ZR_RD_RESET1356 ZR_Rd_Reset1356 = NULL;
ZR_RD_POWEROFF ZR_Rd_PowerOff = NULL;
ZR_RD_REQUEST ZR_Rd_Request = NULL;
ZR_RD_HALT ZR_Rd_Halt = NULL;
ZR_RD_A_S ZR_Rd_A_S = NULL;
ZR_RD_R_A_S ZR_Rd_R_A_S = NULL;
ZR_RD_R_R ZR_Rd_R_R = NULL;
ZR_RD_RATS ZR_Rd_RATS = NULL;
ZR_RD_AUTH ZR_Rd_Auth = NULL;
ZR_RD_READ16 ZR_Rd_Read16 = NULL;
ZR_RD_WRITE16 ZR_Rd_Write16 = NULL;
ZR_RD_WRITE4 ZR_Rd_Write4 = NULL;
ZR_RD_INCREMENT ZR_Rd_Increment = NULL;
ZR_RD_DECREMENT ZR_Rd_Decrement = NULL;
ZR_RD_TRANSFER ZR_Rd_Transfer = NULL;
ZR_RD_RESTORE ZR_Rd_Restore = NULL;
ZR_RD_ISO_MP_4 ZR_Rd_ISO_MP_4 = NULL;
ZR_RD_WRITEKEYTOEEPROM ZR_Rd_WriteKeyToEEPROM = NULL;
ZR_ENCODEMFACCESSBITS ZR_EncodeMfAccessBits = NULL;
ZR_DECODEMFACCESSBITS ZR_DecodeMfAccessBits = NULL;
ZR_M3N_ACTIVATEPOWERKEY ZR_M3n_ActivatePowerKey = NULL;
ZR_M3N_SETOUTPUTS ZR_M3n_SetOutputs = NULL;
ZR_M3N_GETINPUTS ZR_M3n_GetInputs = NULL;
ZR_M3N_SETCONFIG ZR_M3n_SetConfig = NULL;
ZR_M3N_GETCONFIG ZR_M3n_GetConfig = NULL;
ZR_M3N_SETSECURITY ZR_M3n_SetSecurity = NULL;
ZR_M3N_GETSECURITY ZR_M3n_GetSecurity = NULL;
ZR_M3N_SETCLOCK ZR_M3n_SetClock = NULL;
ZR_M3N_GETCLOCK ZR_M3n_GetClock = NULL;
ZR_Z2B_SETFORMAT ZR_Z2b_SetFormat = NULL;
ZR_Z2B_GETFORMAT ZR_Z2b_GetFormat = NULL;
ZR_Z2B_SETPOWERSTATE ZR_Z2b_SetPowerState = NULL;

HMODULE	hZrLib = 0;
INT nZrLoadCount = 0;
#endif // ZGUARD_LINKONREQUEST

BOOL CheckZrVersion()
{
	DWORD nVersion = ZR_GetVersion();
	return ((nVersion & 0xff) == ZR_SDK_VER_MAJOR) && (((nVersion >> 8) & 0xff) == ZR_SDK_VER_MINOR);
}

HRESULT LoadZReader(LPCTSTR pszPath/*=NULL*/)
{
#ifdef ZREADER_LINKONREQUEST
	HRESULT hr = S_OK;
	if (++nZrLoadCount > 1)
		return hr;
	hZrLib = LoadLibrary(pszPath ? pszPath : ZR_DLLNAME);
	if (hZrLib == 0)
		return HRESULT_FROM_WIN32(GetLastError());
	hr = LoadZPort();
	if (SUCCEEDED(hr))
	{
		if (((ZR_GetVersion = reinterpret_cast<ZR_GETVERSION>(GetProcAddress(hZrLib, "ZR_GetVersion"))) != NULL) && 
				CheckZrVersion())
		{
			if (((ZR_Initialize = reinterpret_cast<ZR_INITIALIZE>(GetProcAddress(hZrLib, "ZR_Initialize"))) == NULL) ||
					((ZR_Finalyze = reinterpret_cast<ZR_FINALYZE>(GetProcAddress(hZrLib, "ZR_Finalyze"))) == NULL) ||
					((ZR_UpdateRdFirmware = reinterpret_cast<ZR_UPDATERDFIRMWARE>(GetProcAddress(hZrLib, "ZR_UpdateRdFirmware"))) == NULL) ||
					((ZR_Rd_Open = reinterpret_cast<ZR_RD_OPEN>(GetProcAddress(hZrLib, "ZR_Rd_Open"))) == NULL) ||
					((ZR_Rd_DettachPort = reinterpret_cast<ZR_RD_DETTACHPORT>(GetProcAddress(hZrLib, "ZR_Rd_DettachPort"))) == NULL) ||
					((ZR_Rd_GetConnectionStatus = reinterpret_cast<ZR_RD_GETCONNECTIONSTATUS>(GetProcAddress(hZrLib, "ZR_Rd_GetConnectionStatus"))) == NULL) ||
					((ZR_Rd_GetWaitSettings = reinterpret_cast<ZR_RD_GETWAITSETTINGS>(GetProcAddress(hZrLib, "ZR_Rd_GetWaitSettings"))) == NULL) ||
					((ZR_Rd_SetWaitSettings = reinterpret_cast<ZR_RD_SETWAITSETTINGS>(GetProcAddress(hZrLib, "ZR_Rd_SetWaitSettings"))) == NULL) ||
					((ZR_Rd_SetCapture = reinterpret_cast<ZR_RD_SETCAPTURE>(GetProcAddress(hZrLib, "ZR_Rd_SetCapture"))) == NULL) ||
					((ZR_Rd_ReleaseCapture = reinterpret_cast<ZR_RD_RELEASECAPTURE>(GetProcAddress(hZrLib, "ZR_Rd_ReleaseCapture"))) == NULL) ||
					((ZR_Rd_GetInformation = reinterpret_cast<ZR_RD_GETINFORMATION>(GetProcAddress(hZrLib, "ZR_Rd_GetInformation"))) == NULL) ||
					((ZR_Rd_UpdateFirmware = reinterpret_cast<ZR_RD_UPDATEFIRMWARE>(GetProcAddress(hZrLib, "ZR_Rd_UpdateFirmware"))) == NULL) ||
					((ZR_Rd_SetNotification = reinterpret_cast<ZR_RD_SETNOTIFICATION>(GetProcAddress(hZrLib, "ZR_Rd_SetNotification"))) == NULL) ||
					((ZR_Rd_GetNextMessage = reinterpret_cast<ZR_RD_GETNEXTMESSAGE>(GetProcAddress(hZrLib, "ZR_Rd_GetNextMessage"))) == NULL) ||
					((ZR_Rd_SearchCards = reinterpret_cast<ZR_RD_SEARCHCARDS>(GetProcAddress(hZrLib, "ZR_Rd_SearchCards"))) == NULL) || 
					((ZR_Rd_FindNextCard = reinterpret_cast<ZR_RD_FINDNEXTCARD>(GetProcAddress(hZrLib, "ZR_Rd_FindNextCard"))) == NULL) ||
					((ZR_Rd_SelectCard = reinterpret_cast<ZR_RD_SELECTCARD>(GetProcAddress(hZrLib, "ZR_Rd_SelectCard"))) == NULL) ||
					((ZR_Rd_ReadULCard4Page = reinterpret_cast<ZR_RD_READULCARD4PAGE>(GetProcAddress(hZrLib, "ZR_Rd_ReadULCard4Page"))) == NULL) ||
					((ZR_Rd_WriteULCardPage = reinterpret_cast<ZR_RD_WRITEULCARDPAGE>(GetProcAddress(hZrLib, "ZR_Rd_WriteULCardPage"))) == NULL) ||
					((ZR_Rd_FindT57 = reinterpret_cast<ZR_RD_FINDT57>(GetProcAddress(hZrLib, "ZR_Rd_FindT57"))) == NULL) ||
					((ZR_Rd_ReadT57Block = reinterpret_cast<ZR_RD_READT57BLOCK>(GetProcAddress(hZrLib, "ZR_Rd_ReadT57Block"))) == NULL) ||
					((ZR_Rd_WriteT57Block = reinterpret_cast<ZR_RD_WRITET57BLOCK>(GetProcAddress(hZrLib, "ZR_Rd_WriteT57Block"))) == NULL) ||
					((ZR_Rd_ResetT57 = reinterpret_cast<ZR_RD_RESETT57>(GetProcAddress(hZrLib, "ZR_Rd_ResetT57"))) == NULL) ||
					((ZR_DecodeT57Config = reinterpret_cast<ZR_DECODET57CONFIG>(GetProcAddress(hZrLib, "ZR_DecodeT57Config"))) == NULL) ||
					((ZR_EncodeT57Config = reinterpret_cast<ZR_ENCODET57CONFIG>(GetProcAddress(hZrLib, "ZR_EncodeT57Config"))) == NULL) ||
					((ZR_DecodeT57EmMarine = reinterpret_cast<ZR_DECODET57EMMARINE>(GetProcAddress(hZrLib, "ZR_DecodeT57EmMarine"))) == NULL) ||
					((ZR_EncodeT57EmMarine = reinterpret_cast<ZR_ENCODET57EMMARINE>(GetProcAddress(hZrLib, "ZR_EncodeT57EmMarine"))) == NULL) ||
					((ZR_DecodeT57Hid = reinterpret_cast<ZR_DECODET57HID>(GetProcAddress(hZrLib, "ZR_DecodeT57Hid"))) == NULL) ||
					((ZR_EncodeT57Hid = reinterpret_cast<ZR_ENCODET57HID>(GetProcAddress(hZrLib, "ZR_EncodeT57Hid"))) == NULL) ||
					((ZR_Rd_GetEncodedCardNumber = reinterpret_cast<ZR_RD_GETENCODEDCARDNUMBER>(GetProcAddress(hZrLib, "ZR_Rd_GetEncodedCardNumber"))) == NULL) ||
					((ZR_Rd_AuthorizeSect = reinterpret_cast<ZR_RD_AUTHORIZESECT>(GetProcAddress(hZrLib, "ZR_Rd_AuthorizeSect"))) == NULL) || 
					((ZR_Rd_AuthorizeSectByEKey = reinterpret_cast<ZR_RD_AUTHORIZESECTBYEKEY>(GetProcAddress(hZrLib, "ZR_Rd_AuthorizeSectByEKey"))) == NULL) ||
					((ZR_Rd_ReadMfCardBlock = reinterpret_cast<ZR_RD_READMFCARDBLOCK>(GetProcAddress(hZrLib, "ZR_Rd_ReadMfCardBlock"))) == NULL) ||
					((ZR_Rd_WriteMfCardBlock = reinterpret_cast<ZR_RD_WRITEMFCARDBLOCK>(GetProcAddress(hZrLib, "ZR_Rd_WriteMfCardBlock"))) == NULL) ||
					((ZR_Rd_WriteMfCardBlock4 = reinterpret_cast<ZR_RD_WRITEMFCARDBLOCK4>(GetProcAddress(hZrLib, "ZR_Rd_WriteMfCardBlock4"))) == NULL) ||
					((ZR_Rd_GetIndicatorState = reinterpret_cast<ZR_RD_GETINDICATORSTATE>(GetProcAddress(hZrLib, "ZR_Rd_GetIndicatorState"))) == NULL) ||
					((ZR_Rd_SetIndicatorState = reinterpret_cast<ZR_RD_SETINDICATORSTATE>(GetProcAddress(hZrLib, "ZR_Rd_SetIndicatorState"))) == NULL) ||
					((ZR_Rd_AddIndicatorFlash = reinterpret_cast<ZR_RD_ADDINDICATORFLASH>(GetProcAddress(hZrLib, "ZR_Rd_AddIndicatorFlash"))) == NULL) ||
					((ZR_Rd_BreakIndicatorFlash = reinterpret_cast<ZR_RD_BREAKINDICATORFLASH>(GetProcAddress(hZrLib, "ZR_Rd_BreakIndicatorFlash"))) == NULL) ||
					((ZR_Rd_GetIndicatorFlashAvailable = reinterpret_cast<ZR_RD_GETINDICATORFLASHAVAILABLE>(GetProcAddress(hZrLib, "ZR_Rd_GetIndicatorFlashAvailable"))) == NULL) ||
					((ZR_Rd_Reset1356 = reinterpret_cast<ZR_RD_RESET1356>(GetProcAddress(hZrLib, "ZR_Rd_Reset1356"))) == NULL) ||
					((ZR_Rd_PowerOff = reinterpret_cast<ZR_RD_POWEROFF>(GetProcAddress(hZrLib, "ZR_Rd_PowerOff"))) == NULL) ||
					((ZR_Rd_Request = reinterpret_cast<ZR_RD_REQUEST>(GetProcAddress(hZrLib, "ZR_Rd_Request"))) == NULL) ||
					((ZR_Rd_Halt = reinterpret_cast<ZR_RD_HALT>(GetProcAddress(hZrLib, "ZR_Rd_Halt"))) == NULL) ||
					((ZR_Rd_A_S = reinterpret_cast<ZR_RD_A_S>(GetProcAddress(hZrLib, "ZR_Rd_A_S"))) == NULL) ||
					((ZR_Rd_R_A_S = reinterpret_cast<ZR_RD_R_A_S>(GetProcAddress(hZrLib, "ZR_Rd_R_A_S"))) == NULL) ||
					((ZR_Rd_R_R = reinterpret_cast<ZR_RD_R_R>(GetProcAddress(hZrLib, "ZR_Rd_R_R"))) == NULL) ||
					((ZR_Rd_RATS = reinterpret_cast<ZR_RD_RATS>(GetProcAddress(hZrLib, "ZR_Rd_RATS"))) == NULL) ||
					((ZR_Rd_Auth = reinterpret_cast<ZR_RD_AUTH>(GetProcAddress(hZrLib, "ZR_Rd_Auth"))) == NULL) ||
					((ZR_Rd_Read16 = reinterpret_cast<ZR_RD_READ16>(GetProcAddress(hZrLib, "ZR_Rd_Read16"))) == NULL) || 
					((ZR_Rd_Write16 = reinterpret_cast<ZR_RD_WRITE16>(GetProcAddress(hZrLib, "ZR_Rd_Write16"))) == NULL) ||
					((ZR_Rd_Write4 = reinterpret_cast<ZR_RD_WRITE4>(GetProcAddress(hZrLib, "ZR_Rd_Write4"))) == NULL) ||
					((ZR_Rd_Increment = reinterpret_cast<ZR_RD_INCREMENT>(GetProcAddress(hZrLib, "ZR_Rd_Increment"))) == NULL) ||
					((ZR_Rd_Decrement = reinterpret_cast<ZR_RD_DECREMENT>(GetProcAddress(hZrLib, "ZR_Rd_Decrement"))) == NULL) ||
					((ZR_Rd_Transfer = reinterpret_cast<ZR_RD_TRANSFER>(GetProcAddress(hZrLib, "ZR_Rd_Transfer"))) == NULL) ||
					((ZR_Rd_Restore = reinterpret_cast<ZR_RD_RESTORE>(GetProcAddress(hZrLib, "ZR_Rd_Restore"))) == NULL) ||
					((ZR_Rd_ISO_MP_4 = reinterpret_cast<ZR_RD_ISO_MP_4>(GetProcAddress(hZrLib, "ZR_Rd_ISO_MP_4"))) == NULL) ||
					((ZR_Rd_WriteKeyToEEPROM = reinterpret_cast<ZR_RD_WRITEKEYTOEEPROM>(GetProcAddress(hZrLib, "ZR_Rd_WriteKeyToEEPROM"))) == NULL) ||
					((ZR_EncodeMfAccessBits = reinterpret_cast<ZR_ENCODEMFACCESSBITS>(GetProcAddress(hZrLib, "ZR_EncodeMfAccessBits"))) == NULL) ||
					((ZR_DecodeMfAccessBits = reinterpret_cast<ZR_DECODEMFACCESSBITS>(GetProcAddress(hZrLib, "ZR_DecodeMfAccessBits"))) == NULL) ||
					((ZR_M3n_ActivatePowerKey = reinterpret_cast<ZR_M3N_ACTIVATEPOWERKEY>(GetProcAddress(hZrLib, "ZR_M3n_ActivatePowerKey"))) == NULL) ||
					((ZR_M3n_SetOutputs = reinterpret_cast<ZR_M3N_SETOUTPUTS>(GetProcAddress(hZrLib, "ZR_M3n_SetOutputs"))) == NULL) ||
					((ZR_M3n_GetInputs = reinterpret_cast<ZR_M3N_GETINPUTS>(GetProcAddress(hZrLib, "ZR_M3n_GetInputs"))) == NULL) ||
					((ZR_M3n_SetConfig = reinterpret_cast<ZR_M3N_SETCONFIG>(GetProcAddress(hZrLib, "ZR_Finalyze"))) == NULL) ||
					((ZR_M3n_GetConfig = reinterpret_cast<ZR_M3N_GETCONFIG>(GetProcAddress(hZrLib, "ZR_M3n_GetConfig"))) == NULL) ||
					((ZR_M3n_SetSecurity = reinterpret_cast<ZR_M3N_SETSECURITY>(GetProcAddress(hZrLib, "ZR_M3n_SetSecurity"))) == NULL) ||
					((ZR_M3n_GetSecurity = reinterpret_cast<ZR_M3N_GETSECURITY>(GetProcAddress(hZrLib, "ZR_M3n_GetSecurity"))) == NULL) ||
					((ZR_M3n_SetClock = reinterpret_cast<ZR_M3N_SETCLOCK>(GetProcAddress(hZrLib, "ZR_M3n_SetClock"))) == NULL) ||
					((ZR_M3n_GetClock = reinterpret_cast<ZR_M3N_GETCLOCK>(GetProcAddress(hZrLib, "ZR_M3n_GetClock"))) == NULL) ||
					((ZR_Z2b_SetFormat = reinterpret_cast<ZR_Z2B_SETFORMAT>(GetProcAddress(hZrLib, "ZR_Z2b_SetFormat"))) == NULL) ||
					((ZR_Z2b_GetFormat = reinterpret_cast<ZR_Z2B_GETFORMAT>(GetProcAddress(hZrLib, "ZR_Z2b_GetFormat"))) == NULL) ||
					((ZR_Z2b_SetPowerState = reinterpret_cast<ZR_Z2B_SETPOWERSTATE>(GetProcAddress(hZrLib, "ZR_Z2b_SetPowerState"))) == NULL))
				hr = E_NOINTERFACE;
		}
		else
			hr = E_NOINTERFACE;
	}

	if (FAILED(hr))
		UnloadZReader();
	return hr;
#else
	if (CheckZrVersion())
		return S_OK;
	else
		return E_NOINTERFACE;
#endif // ZREADER_LINKONREQUEST
}

VOID UnloadZReader()
{
#ifdef ZREADER_LINKONREQUEST
	if (--nZrLoadCount > 0)
		return;
	FreeLibrary(hZrLib);
	hZrLib = NULL;
	UnloadZPort();
	ZR_GetVersion = NULL;
	ZR_Initialize = NULL;
	ZR_Finalyze = NULL;

	ZR_UpdateRdFirmware = NULL;
	ZR_Rd_Open = NULL;
	ZR_Rd_DettachPort = NULL;
	ZR_Rd_GetConnectionStatus = NULL;
	ZR_Rd_GetWaitSettings = NULL;
	ZR_Rd_SetWaitSettings = NULL;
	ZR_Rd_SetCapture = NULL;
	ZR_Rd_ReleaseCapture = NULL;
	ZR_Rd_GetInformation = NULL;
	ZR_Rd_UpdateFirmware = NULL;
	ZR_Rd_SetNotification = NULL;
	ZR_Rd_GetNextMessage = NULL;
	ZR_Rd_SearchCards = NULL;
	ZR_Rd_FindNextCard = NULL;
	ZR_Rd_SelectCard = NULL;
	ZR_Rd_ReadULCard4Page = NULL;
	ZR_Rd_WriteULCardPage = NULL;
	ZR_Rd_FindT57 = NULL;
	ZR_Rd_ReadT57Block = NULL;
	ZR_Rd_WriteT57Block = NULL;
	ZR_Rd_ResetT57 = NULL;
	ZR_DecodeT57Config = NULL;
	ZR_EncodeT57Config = NULL;
	ZR_DecodeT57EmMarine = NULL;
	ZR_EncodeT57EmMarine = NULL;
	ZR_DecodeT57Hid = NULL;
	ZR_EncodeT57Hid = NULL;
	ZR_Rd_GetEncodedCardNumber = NULL;
	ZR_Rd_AuthorizeSect = NULL;
	ZR_Rd_AuthorizeSectByEKey = NULL;
	ZR_Rd_ReadMfCardBlock = NULL;
	ZR_Rd_WriteMfCardBlock = NULL;
	ZR_Rd_WriteMfCardBlock4 = NULL;
	ZR_Rd_GetIndicatorState = NULL;
	ZR_Rd_SetIndicatorState = NULL;
	ZR_Rd_AddIndicatorFlash = NULL;
	ZR_Rd_BreakIndicatorFlash = NULL;
	ZR_Rd_GetIndicatorFlashAvailable = NULL;
	ZR_Rd_Reset1356 = NULL;
	ZR_Rd_PowerOff = NULL;
	ZR_Rd_Request = NULL;
	ZR_Rd_Halt = NULL;
	ZR_Rd_A_S = NULL;
	ZR_Rd_R_A_S = NULL;
	ZR_Rd_R_R = NULL;
	ZR_Rd_RATS = NULL;
	ZR_Rd_Auth = NULL;
	ZR_Rd_Read16 = NULL;
	ZR_Rd_Write16 = NULL;
	ZR_Rd_Write4 = NULL;
	ZR_Rd_Increment = NULL;
	ZR_Rd_Decrement = NULL;
	ZR_Rd_Transfer = NULL;
	ZR_Rd_Restore = NULL;
	ZR_Rd_ISO_MP_4 = NULL;
	ZR_Rd_WriteKeyToEEPROM = NULL;
	ZR_EncodeMfAccessBits = NULL;
	ZR_DecodeMfAccessBits = NULL;
	ZR_M3n_ActivatePowerKey = NULL;
	ZR_M3n_SetOutputs = NULL;
	ZR_M3n_GetInputs = NULL;
	ZR_M3n_SetConfig = NULL;
	ZR_M3n_GetConfig = NULL;
	ZR_M3n_SetSecurity = NULL;
	ZR_M3n_GetSecurity = NULL;
	ZR_M3n_SetClock = NULL;
	ZR_M3n_GetClock = NULL;
	ZR_Z2b_SetFormat = NULL;
	ZR_Z2b_GetFormat = NULL;
	ZR_Z2b_SetPowerState = NULL;
#endif // ZREADER_LINKONREQUEST
}
