#pragma once
// Приведенный ниже блок ifdef - это стандартный метод создания макросов, упрощающий процедуру 
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа ZREADER_EXPORTS,
// в командной строке. Этот символ не должен быть определен в каком-либо проекте
// использующем данную DLL. Благодаря этому любой другой проект, чьи исходные файлы включают данный файл, видит 
// функции ZREADER_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.

// (rom) this is the switch to change between static and dynamic linking.
// (rom) it is enabled by default here.
// (rom) To disable simply change the '$' to a '.'.
//#define ZREADER_LINKONREQUEST


#ifndef ZREADER_LINKONREQUEST

#ifdef ZREADER_EXPORTS
#define ZREADER_API(type) extern "C"  __declspec(dllexport) type __stdcall 
#else
#ifdef ZREADER_STATIC
#define ZREADER_API(type) extern "C" type __stdcall 
#else
#define ZREADER_API(type) extern "C" __declspec(dllimport) type __stdcall
#endif
#endif
#else
#include <tchar.h>
#endif // !ZREADER_LINKONREQUEST

#include "ZPort.h"
#include "ZBase.h"
#include "ZRErrors.h"

#define ZR_SDK_VER_MAJOR	3
#define ZR_SDK_VER_MINOR	26

#pragma pack(1)   // turn byte alignment on

#define ZR_IF_LOG						0x100	// Записывать лог

#define ZR_DEVTYPE_CVT					0
#define ZR_DEVTYPE_Z2U					3
#define ZR_DEVTYPE_Z2M					4
#define ZR_DEVTYPE_Z2M2					8
#define ZR_DEVTYPE_Z2B					6
#define ZR_DEVTYPE_EHR					7
#define ZR_DEVTYPE_READERS				0x1D9

#define ZR_DEVTYPE_IPGUARD				(ZP_MAX_REG_DEV)
#define ZR_IPDEVTYPE_CVTS				1

// Флаги для _ZR_RD_NOTIFY_SETTINGS.nNMask
#define ZR_RNF_EXIST_CARD				1		// ZR_RN_CARD_INSERT / ZR_RN_CARD_REMOVE
#define ZR_RNF_INPUT_CHANGE				2		// ZR_RN_INPUT_CHANGE
#define ZR_RNF_IND_FLASH_END			4		// ZR_RN_IND_FLASH_END
#define ZR_RNF_ERROR					8		// ZR_RN_ERROR
#define ZR_RDNF_CONNECTION_CHANGE		0x10	// ZR_RDN_CONNECTION_CHANGE

// Уведомления функции ZR_FindNotification
#define ZR_RN_CARD_INSERT				100	// Карта поднесена ((PZR_CARD_INFO)nMsgParam)
#define ZR_RN_CARD_REMOVE				101 // Карта удалена ((PZR_CARD_INFO)nMsgParam, может быть = NULL)
#define ZR_RN_CARD_UNKNOWN				102	// Неизвестное сообщение от считывателя, (LPWCSTR)nMsgParam - текст сообщения
#define ZR_RN_INPUT_CHANGE				103 // Изменилось состояние входов Matrix III Net (nMsgParam - новое состояние входов)
#define ZR_RN_IND_FLASH_END				104	// Очередь индикации завершена (Z-2 USB MF, CPZ-2-MF, Matrix III Net), без параметра
#define ZR_RN_ERROR						105 // Произошла ошибка в ните (HRESULT*)nMsgParam - код ошибки
#define ZR_RDN_CONNECTION_CHANGE		106 // Изменилось состояние подключения

// Значения по умолчанию
#define ZR_MIF_CHECKCARDPERIOD			300	// Период сканирования карт в поле считывателей, работающий по протоколу "mif" (Z-2 USB MF, Matrix III Net и CP-Z-2MF)
#define ZR_CHECKCARDPERIOD				INFINITE // Период сканирования карт в поле всех считывателей, кроме тех, которые работают по протоколу "mif"
#define ZR_M3N_CHECKINPUTPERIOD			INFINITE // Период проверки состояния входов считывателя Matrix III Net
#define ZR_SEARCH_MAXCARD	16	// Предел найденных карт при поиске (значение по умолчанию для ZR_Rd_SearchCards)

// Тип считывателя
enum ZR_RD_TYPE: INT
{
	ZR_RD_UNDEF = 0,
	ZR_RD_Z2U,				// Z-2 USB
	ZR_RD_M3A,				// Matrix III Rd-All
	ZR_RD_Z2M,				// Z-2 USB MF
	ZR_RD_M3N,				// Matrix III Net
	ZR_RD_CPZ2MF,			// CP-Z-2MF
	ZR_RD_Z2EHR,			// Z-2 EHR
	ZR_RD_Z2BASE,			// Z-2 Base
	ZR_RD_M5				// Matrix V
};

// Настройки уведомлений считывателя
typedef struct _ZR_RD_NOTIFY_SETTINGS
{
	UINT nNMask;				// Маска типов уведомлений (ZR_RNF_..)

	HANDLE hEvent;				// Событие (объект синхронизации)
	HWND hWindow;				// Окно, принимиющее сообщение nWndMsgId
	UINT nWndMsgId;				// Сообщение для отправки окну hWnd

	UINT nCheckCardPeriod;		// Период сканирования карт в поле считывателя в миллисекундах. Если =0, используется значение по умолчанию
	UINT nCheckInputPeriod;		// Период проверки состояния входов для Matrix III Net. Если =0, используется значение ZR_M3N_CHECKINPUTPERIOD
} *PZR_RD_NOTIFY_SETTINGS;

// Информация о считывателе
typedef struct _ZR_RD_INFO : _ZP_DEVICE_INFO
{
	ZR_RD_TYPE nType;		// Тип считывателя

	LPWSTR pszLinesBuf;		// Буфер для информационных строк
	INT nLinesBufMax;		// Размер буфера в символах, включая завершающий '\0'
} *PZR_RD_INFO;

#ifndef ZG_DEVTYPE_CVTS
// Модель конвертера
enum ZG_CVT_TYPE : INT
{
	ZG_CVT_UNDEF = 0,		// Не определено
	ZG_CVT_Z397,			// Z-397
	ZG_CVT_Z397_GUARD,		// Z-397 Guard
	ZG_CVT_Z397_IP,			// Z-397 IP
	ZG_CVT_Z397_WEB,		// Z-397 Web
	ZG_CVT_Z5R_WEB			// Z5R Web
};

// Режим конвертера Z397 Guard
enum ZG_GUARD_MODE : INT
{
	ZG_GUARD_UNDEF = 0,		// Не определено
	ZG_GUARD_NORMAL,		// Режим "Normal" (эмуляция обычного конвертера Z397)
	ZG_GUARD_ADVANCED,		// Режим "Advanced"
	ZG_GUARD_TEST,			// Режим "Test" (для специалистов)
	ZG_GUARD_ACCEPT			// Режим "Accept" (для специалистов)
};
typedef struct _ZG_ENUM_IPCVT_INFO : _ZP_DEVICE_INFO
{
	ZG_CVT_TYPE nType;		// Модель IP-конвертера
	ZG_GUARD_MODE nMode;	// Режим работы конвертера Guard
	DWORD nFlags;			// Флаги: бит 0 - "VCP", бит 1 - "WEB", 0xFF - "All"
} *PZG_ENUM_IPCVT_INFO;
#endif // !ZG_DEVTYPE_CVTS

// Флаги для nFlags стуктуры ZR_RD_OPEN_PARAMS
#define ZR_RD_WIEGAND		2	// Z-2 Base: подключение по Wiegand

// Параметры открытия считывателя (для функции ZR_Rd_Open)
typedef struct _ZR_RD_OPEN_PARAMS
{
	ZP_PORT_TYPE nType;			// Тип порта
	LPCWSTR pszName;			// Имя порта. Если =NULL, то используется hPort
	HANDLE hPort;				// Дескриптор порта, полученный функцией ZP_Open
	ZR_RD_TYPE nRdType;			// Тип считывателя. Если =ZR_RD_UNDEF, то автоопределение
	PZP_WAIT_SETTINGS pWait;	// Параметры ожидания. Может быть =NULL.
	BYTE nStopBits;
	UINT nFlags;
} *PZR_RD_OPEN_PARAMS;

// Тип карты
enum ZR_CARD_TYPE: INT
{
	ZR_CD_UNDEF = 0,
	ZR_CD_EM,				// Em-Marine
	ZR_CD_HID,				// Hid
	ZR_CD_IC,				// iCode
	ZR_CD_UL,				// Mifare UltraLight
	ZR_CD_1K,				// Mifare Classic 1K
	ZR_CD_4K,				// Mifare Classic 4K
	ZR_CD_DF,				// Mifare DESFire
	ZR_CD_PX,				// Mifare ProX
	ZR_CD_COD433F,			// Cod433 Fix
	ZR_CD_COD433,			// Cod433
	ZR_CD_DALLAS,			// Dallas
	ZR_CD_CAME433,			// радиобрелок CAME
	ZR_CD_PLUS,				// Mifare Plus
	ZR_CD_PLUS1K,			// Mifare Plus 1K
	ZR_CD_PLUS2K,			// Mifare Plus 2K
	ZR_CD_PLUS4K,			// Mifare Plus 4K
	ZR_CD_MINI				// Mifare Mini
};

typedef struct _ZR_CARD_INFO
{
	ZR_CARD_TYPE nType;		// Тип карты
	Z_KEYNUM nNum;			// Номер карты
} *PZR_CARD_INFO;

// Флаги для функции ZR_Rd_FindT57 \ ZR_Rd_ReadT57Block \ ZR_Rd_WriteT57Block
enum _ZR_RD_T57_FLAGS
{
	ZR_T57F_INIT		= 0x0001,	// Разрешить инициализировать, если не удалось подобрать параметры (только в ZR_Rd_FindT57)
	ZR_T57F_PSW			= 0x0002,	// Использовать пароль
	ZR_T57F_BLOCK		= 0x0004	// Блокировать дальнейшую перезапись блока (только в ZR_Rd_WriteT57Block)
};
typedef UINT ZR_T57_FLAGS;

// Modulation
#define T57_MOD_DIRECT			0    // 0 0 0 0 0
#define T57_MOD_PSK1			2    // 0 0 0 1 0
#define T57_MOD_PSK2			4    // 0 0 1 0 0
#define T57_MOD_PSK3			6    // 0 0 1 1 0
#define T57_MOD_FSK1			8    // 0 1 0 0 0
#define T57_MOD_FSK2			0xA  // 0 1 0 1 0
#define T57_MOD_FSK1A			0xC  // 0 1 1 0 0
#define T57_MOD_FSK2A			0xE  // 0 1 1 1 0
#define T57_MOD_MANCHESTER		0x10 // 1 0 0 0 0
#define T57_MOD_BIPHASE50		0x1  // 0 0 0 0 1
#define T57_MOD_BIPHASE57		0x11 // 1 0 0 0 1
// PSK CF
enum ZR_T57_PSK: INT
{
	T57_PSK_UNDEF = -1,
	T57_PSK_RF2,	// 0 0
	T57_PSK_RF4,	// 0 1
	T57_PSK_RF8,	// 1 0
	T57_PSK_RES		// 1 1
};

typedef struct _ZR_T57_CONFIG
{
	BOOL fXMode;			// True, если X-Mode, иначе - e5550 compatible
	UINT nMasterKey;
	UINT nDataRate;
	UINT nModulation;
	ZR_T57_PSK nPSK_CF;
	BOOL fAOR;
	BOOL fOTP;				// (только в XMode)
	INT nMaxBlock;
	BOOL fPsw;				// True, если пароль установлен
	BOOL fST_Seq_Ter;		// (только в e5550)
	BOOL fSST_Seq_StMrk;	// (только в XMode)
	BOOL fFastWrite;
	BOOL fInverseData;		// (только в XMode)
	BOOL fPOR_Delay;
} *PZR_T57_CONFIG;

typedef struct _ZR_M3N_CONFIG
{
	BYTE nWorkMode;
	BYTE nOutZumm;
	BYTE nOutTM;
	BYTE nOutExit;
	BYTE nOutLock;
	BYTE nOutDoor;
	BYTE nProt;
	BYTE nFlags;	// 0 Impulse, 1 No card, 2 card num
	BYTE nCardFormat;
	BYTE nSecurityMode;
	BYTE Reserved1[2];
} *PZR_M3N_CONFIG;

typedef UINT64 ZR_MF_AUTH_KEY[2];
typedef ZR_MF_AUTH_KEY near *PZR_MF_AUTH_KEY;

enum ZR_IND_STATE: INT
{
	ZR_IND_NO_CHANGE = 0,
	ZR_IND_ON,
	ZR_IND_OFF,
	ZR_IND_AUTO
};

// Флаги состояния индикатора
enum _ZR_RD_IND_FLAGS
{
	ZR_ISF_ON			= 0x0001,
	ZR_ISF_AUTO			= 0x0002
};
typedef UINT ZR_IND_FLAGS;

typedef struct _ZR_IND_FLASH
{
	ZR_IND_STATE nRed;
	ZR_IND_STATE nGreen;
	ZR_IND_STATE nSound;
	DWORD nDuration; // ms
} *PZR_IND_FLASH;

#define ZR_MAX_IND_FLASH		15

typedef BOOL (CALLBACK* ZR_PROCESSCALLBACK) (INT nPos, INT nMax, PVOID pUserData);

// Устаревшие типы
typedef BOOL (CALLBACK* ZR_ENUMDRDPROC) (PZR_RD_INFO pInfo, PZP_PORT_INFO pPort, PVOID pUserData);
typedef BOOL (CALLBACK* ZR_ENUMCARDSPROC) (PZR_CARD_INFO pInfo, PVOID pUserData);

#pragma pack() // turn byte alignment off

#ifndef ZREADER_LINKONREQUEST

// Возвращает версию библиотеки Z2Usb.dll
ZREADER_API(DWORD) ZR_GetVersion();

// Инициализация/финализация библиотеки
ZREADER_API(HRESULT) ZR_Initialize(UINT nFlags);
ZREADER_API(HRESULT) ZR_Finalyze();

// Загружает новую прошивку в считыватель
ZREADER_API(HRESULT) ZR_UpdateRdFirmware(PZR_RD_OPEN_PARAMS pParams,   
	LPCVOID pData, INT nCount, ZR_PROCESSCALLBACK pfnCallback, PVOID pUserData);

// Открывает/закрывает считыватель
ZREADER_API(HRESULT) ZR_Rd_Open(PHANDLE pHandle, PZR_RD_OPEN_PARAMS pParams, 
	PZR_RD_INFO pInfo=NULL);
// Отключается от считывателя, не закрывая порт, возвращает дескриптор порта, полученный функцией ZP_Open
ZREADER_API(HRESULT) ZR_Rd_DettachPort(HANDLE hHandle, PHANDLE pPortHandle);
// Возвращает состояние подключения
ZREADER_API(HRESULT) ZR_Rd_GetConnectionStatus(HANDLE hHandle, ZP_CONNECTION_STATUS* pValue);

// Возвращает/устанавливает параметры ожидания исполнения функции
ZREADER_API(HRESULT) ZR_Rd_GetWaitSettings(HANDLE hHandle, PZP_WAIT_SETTINGS pSetting);
ZREADER_API(HRESULT) ZR_Rd_SetWaitSettings(HANDLE hHandle, PZP_WAIT_SETTINGS pSetting);

// Захватить порт (приостанавливает фоновые операции с портом)
ZREADER_API(HRESULT) ZR_Rd_SetCapture(HANDLE hHandle);
// Отпустить порт (возобновляет фоновые операции с портом)
ZREADER_API(HRESULT) ZR_Rd_ReleaseCapture(HANDLE hHandle);

// Возвращает информацию о считывателе
ZREADER_API(HRESULT) ZR_Rd_GetInformation(HANDLE hHandle, PZR_RD_INFO pInfo);

// Загружает в считыватель новую прошивку
ZREADER_API(HRESULT) ZR_Rd_UpdateFirmware(HANDLE hHandle, LPCVOID pData, INT nCount, 
	ZR_PROCESSCALLBACK pfnCallback, PVOID pUserData);

// Настраивает уведомления считывателя
ZREADER_API(HRESULT) ZR_Rd_SetNotification(HANDLE hHandle, PZR_RD_NOTIFY_SETTINGS pSettings);
ZREADER_API(HRESULT) ZR_Rd_GetNextMessage(HANDLE hHandle, PUINT pMsg, LPARAM* pMsgParam);

// Поиск карт в поле считывателя
#define ZR_RSCF_DETECTOR	1	// Использовать уже готовый список найденных карт детектора
ZREADER_API(HRESULT) ZR_Rd_SearchCards(HANDLE hHandle, INT nMaxCard=ZR_SEARCH_MAXCARD, UINT nFlags=0);
ZREADER_API(HRESULT) ZR_Rd_FindNextCard(HANDLE hHandle, PZR_CARD_INFO pInfo);
ZREADER_API(HRESULT) ZR_Rd_SelectCard(HANDLE hHandle, Z_KEYNUM* pNum, ZR_CARD_TYPE nType);

// Чтение/запись карты Mifare UL
ZREADER_API(HRESULT) ZR_Rd_ReadULCard4Page(HANDLE hHandle, INT nPageN, LPVOID pBuf16);
ZREADER_API(HRESULT) ZR_Rd_WriteULCardPage(HANDLE hHandle, INT nPageN, LPCVOID pData4);


// Z2USB

ZREADER_API(HRESULT) ZR_Rd_FindT57(HANDLE hHandle, Z_KEYNUM* pNum, PINT pPar, UINT nPsw, ZR_T57_FLAGS nFlags);
ZREADER_API(HRESULT) ZR_Rd_ReadT57Block(HANDLE hHandle, INT nBlockN, LPVOID pBuf4, INT nPar, UINT nPsw, ZR_T57_FLAGS nFlags);
ZREADER_API(HRESULT) ZR_Rd_WriteT57Block(HANDLE hHandle, INT nBlockN, LPCVOID pData4, INT nPar, UINT nPsw, ZR_T57_FLAGS nFlags);
ZREADER_API(HRESULT) ZR_Rd_ResetT57(HANDLE hHandle);

ZREADER_API(HRESULT) ZR_DecodeT57Config(PZR_T57_CONFIG pConfig, LPCVOID pData4);
ZREADER_API(HRESULT) ZR_EncodeT57Config(LPVOID pBuf4, PZR_T57_CONFIG pConfig);
ZREADER_API(HRESULT) ZR_DecodeT57EmMarine(PINT pBitOffs, Z_KEYNUM* pNum, LPCVOID pData, INT nCount);
ZREADER_API(HRESULT) ZR_EncodeT57EmMarine(LPVOID pBuf8_, INT nBufSize, INT nBitOffs, Z_KEYNUM* pNum);
ZREADER_API(HRESULT) ZR_DecodeT57Hid(Z_KEYNUM* pNum, LPCVOID pData12, PINT pWiegand);
ZREADER_API(HRESULT) ZR_EncodeT57Hid(LPVOID pBuf12, Z_KEYNUM* pNum, INT nWiegand);

ZREADER_API(HRESULT) ZR_Rd_GetEncodedCardNumber(HANDLE hHandle, ZR_CARD_TYPE* pType, Z_KEYNUM* pNum, LPVOID pBuf, INT nBufSize, LPINT pRCount);

// Z2USB MF и Matrix-III Net

ZREADER_API(HRESULT) ZR_Rd_AuthorizeSect(HANDLE hHandle, INT nBlockN, const ZR_MF_AUTH_KEY* pKey, UINT nFlags=0);
ZREADER_API(HRESULT) ZR_Rd_AuthorizeSectByEKey(HANDLE hHandle, INT nBlockN, UINT nKeyMask, PINT pRKeyIdx, UINT nFlags=0);
ZREADER_API(HRESULT) ZR_Rd_ReadMfCardBlock(HANDLE hHandle, INT nBlockN, LPVOID pBuf16, UINT nFlags=0);
ZREADER_API(HRESULT) ZR_Rd_WriteMfCardBlock(HANDLE hHandle, INT nBlockN, LPCVOID pData16, UINT nFlags=0);
ZREADER_API(HRESULT) ZR_Rd_WriteMfCardBlock4(HANDLE hHandle, INT nBlockN, LPCVOID pData4);

ZREADER_API(HRESULT) ZR_Rd_GetIndicatorState(HANDLE hHandle, ZR_IND_FLAGS* pRed, ZR_IND_FLAGS* pGreen, ZR_IND_FLAGS* pSound);
ZREADER_API(HRESULT) ZR_Rd_SetIndicatorState(HANDLE hHandle, ZR_IND_STATE nRed, ZR_IND_STATE nGreen, ZR_IND_STATE nSound);
ZREADER_API(HRESULT) ZR_Rd_AddIndicatorFlash(HANDLE hHandle, PZR_IND_FLASH pRecs, INT nCount, BOOL fReset, PINT pRCount);
ZREADER_API(HRESULT) ZR_Rd_BreakIndicatorFlash(HANDLE hHandle, BOOL fAutoMode);
ZREADER_API(HRESULT) ZR_Rd_GetIndicatorFlashAvailable(HANDLE hHandle, PINT pCount);

// Команды управления (состояние: считывателя, светодиодов)

ZREADER_API(HRESULT) ZR_Rd_Reset1356(HANDLE hHandle, WORD nWaitMs);
ZREADER_API(HRESULT) ZR_Rd_PowerOff(HANDLE hHandle);

// Команды ISO

ZREADER_API(HRESULT) ZR_Rd_Request(HANDLE hHandle, BOOL fWakeUp, ZR_CARD_TYPE* pType, PWORD pATQ);
ZREADER_API(HRESULT) ZR_Rd_Halt(HANDLE hHandle);
ZREADER_API(HRESULT) ZR_Rd_A_S(HANDLE hHandle, Z_KEYNUM* pNum, PBYTE pSAK);
ZREADER_API(HRESULT) ZR_Rd_R_A_S(HANDLE hHandle, BOOL fWakeUp, ZR_CARD_TYPE* pType, Z_KEYNUM* pNum, PBYTE pSAK, PWORD pATQ);
ZREADER_API(HRESULT) ZR_Rd_R_R(HANDLE hHandle, Z_KEYNUM* pNum, BOOL fWakeUp);
ZREADER_API(HRESULT) ZR_Rd_RATS(HANDLE hHandle, LPVOID pBuf, INT nBufSize, LPINT pRCount);
// Общие флаги карт Mifare
#define ZR_MFF_MP		0x100	// карта - Mifare Plus (иначе - старый Mifare)
// Флаги авторизации карт Mifare
#define ZR_MFAF_B		1	// Авторизация по ключу B (иначе - по ключу A)
#define ZR_MFAF_KEY		2	// Авторизация по указанному ключу (иначе - по ключам в памяти считывателя)
#define ZR_MFAF_FA		0x10 // Following authenticate (для Mifare Plus)
#define ZR_MFAF_RA		0x30 // Reset authentication (для Mifare Plus)
// Флаги чтения/записи Mifare Plus
#define ZR_MFRF_RESP	1	// MAC on responce
#define ZR_MFRF_OPEN	2	// открытый текст (иначе - закрытый текст)
#define ZR_MFRF_CMD		4	// MAC on command

ZREADER_API(HRESULT) ZR_Rd_Auth(HANDLE hHandle, UINT nAddr, PZR_MF_AUTH_KEY pKey, UINT nEKeyMask, PINT pEKeyIdx, UINT nFlags);
ZREADER_API(HRESULT) ZR_Rd_Read16(HANDLE hHandle, UINT nAddr, LPVOID pBuf16, UINT nFlags);
ZREADER_API(HRESULT) ZR_Rd_Write16(HANDLE hHandle, UINT nAddr, LPCVOID pData16, UINT nFlags);
// Запись блока 4
ZREADER_API(HRESULT) ZR_Rd_Write4(HANDLE hHandle, UINT nAddr, LPCVOID pData4);

ZREADER_API(HRESULT) ZR_Rd_Increment(HANDLE hHandle, UINT nAddr, UINT nValue);
ZREADER_API(HRESULT) ZR_Rd_Decrement(HANDLE hHandle, UINT nAddr, UINT nValue);
ZREADER_API(HRESULT) ZR_Rd_Transfer(HANDLE hHandle, UINT nAddr);
ZREADER_API(HRESULT) ZR_Rd_Restore(HANDLE hHandle, UINT nAddr);
ZREADER_API(HRESULT) ZR_Rd_ISO_MP_4(HANDLE hHandle, LPCVOID pData, INT nCount, LPVOID pBuf, INT nBufSize, LPINT pRCount);
// Запись ключа в EEPROM
ZREADER_API(HRESULT) ZR_Rd_WriteKeyToEEPROM(HANDLE hHandle, UINT nAddr, BOOL fKeyB, PZR_MF_AUTH_KEY pKey);

ZREADER_API(HRESULT) ZR_EncodeMfAccessBits(INT nAreaN, LPVOID pBuf3, UINT nBits);
ZREADER_API(HRESULT) ZR_DecodeMfAccessBits(INT nAreaN, PUINT pBits, LPCVOID pData3);

// Matrix-III Net

// Активация силового ключа
ZREADER_API(HRESULT) ZR_M3n_ActivatePowerKey(HANDLE hHandle, BOOL fForce, UINT nTimeMs);
// Управление выходами (если разрешено параметрами)
ZREADER_API(HRESULT) ZR_M3n_SetOutputs(HANDLE hHandle, UINT nMask, UINT nOutBits);
// Считать входы
ZREADER_API(HRESULT) ZR_M3n_GetInputs(HANDLE hHandle, PUINT pFlags);
// Установить параметры
ZREADER_API(HRESULT) ZR_M3n_SetConfig(HANDLE hHandle, PZR_M3N_CONFIG pConfig);
// Считать параметры
ZREADER_API(HRESULT) ZR_M3n_GetConfig(HANDLE hHandle, PZR_M3N_CONFIG pConfig);
// Установить параметры security
ZREADER_API(HRESULT) ZR_M3n_SetSecurity(HANDLE hHandle, INT nBlockN, UINT nKeyMask, BOOL fKeyB);
// Получить параметры security
ZREADER_API(HRESULT) ZR_M3n_GetSecurity(HANDLE hHandle, PINT pBlockN, PUINT pKeyMask, PBOOL pKeyB);
// Установить часы
ZREADER_API(HRESULT) ZR_M3n_SetClock(HANDLE hHandle, LPSYSTEMTIME pTime);
// Получить часы
ZREADER_API(HRESULT) ZR_M3n_GetClock(HANDLE hHandle, LPSYSTEMTIME pTime);

// Z-2 Base
ZREADER_API(HRESULT) ZR_Z2b_SetFormat(HANDLE hHandle, LPCWSTR pszFmt, LPCWSTR pszArg, LPCWSTR pszNoCard, BOOL fSaveEE);
ZREADER_API(HRESULT) ZR_Z2b_GetFormat(HANDLE hHandle, LPWSTR szFmtBuf, INT nFmtBufSize, 
	LPWSTR szArgBuf, INT nArgBufSize, LPWSTR szNCBuf, INT nNCBufSize);
ZREADER_API(HRESULT) ZR_Z2b_SetPowerState(HANDLE hHandle, BOOL fOn);

#else

#define ZR_DLLNAME       _T("ZReader.dll")

extern "C"
{
	typedef DWORD (__stdcall* ZR_GETVERSION)();
	typedef HRESULT (__stdcall* ZR_INITIALIZE)(UINT nFlags);
	typedef HRESULT (__stdcall* ZR_FINALYZE)();

	typedef HRESULT (__stdcall* ZR_UPDATERDFIRMWARE)(PZR_RD_OPEN_PARAMS pParams,   
		LPCVOID pData, INT nCount, ZR_PROCESSCALLBACK pfnCallback, PVOID pUserData);
	typedef HRESULT (__stdcall* ZR_RD_OPEN)(PHANDLE pHandle, PZR_RD_OPEN_PARAMS pParams, 
		PZR_RD_INFO pInfo/*=NULL*/);
	typedef HRESULT (__stdcall* ZR_RD_DETTACHPORT)(HANDLE hHandle, PHANDLE pPortHandle);
	typedef HRESULT (__stdcall* ZR_RD_GETCONNECTIONSTATUS)(HANDLE hHandle, ZP_CONNECTION_STATUS* pValue);
	typedef HRESULT (__stdcall* ZR_RD_GETWAITSETTINGS)(HANDLE hHandle, PZP_WAIT_SETTINGS pSetting);
	typedef HRESULT (__stdcall* ZR_RD_SETWAITSETTINGS)(HANDLE hHandle, PZP_WAIT_SETTINGS pSetting);
	typedef HRESULT (__stdcall* ZR_RD_SETCAPTURE)(HANDLE hHandle);
	typedef HRESULT (__stdcall* ZR_RD_RELEASECAPTURE)(HANDLE hHandle);
	typedef HRESULT (__stdcall* ZR_RD_GETINFORMATION)(HANDLE hHandle, PZR_RD_INFO pInfo);
	typedef HRESULT (__stdcall* ZR_RD_UPDATEFIRMWARE)(HANDLE hHandle, LPCVOID pData, INT nCount, 
		ZR_PROCESSCALLBACK pfnCallback, PVOID pUserData);
	typedef HRESULT (__stdcall* ZR_RD_SETNOTIFICATION)(HANDLE hHandle, PZR_RD_NOTIFY_SETTINGS pSettings);
	typedef HRESULT (__stdcall* ZR_RD_GETNEXTMESSAGE)(HANDLE hHandle, PUINT pMsg, LPARAM* pMsgParam);
	typedef HRESULT (__stdcall* ZR_RD_SEARCHCARDS)(HANDLE hHandle, INT nMaxCard/*=ZR_SEARCH_MAXCARD*/, UINT nFlags/*=0*/);
	typedef HRESULT (__stdcall* ZR_RD_FINDNEXTCARD)(HANDLE hHandle, PZR_CARD_INFO pInfo);
	typedef HRESULT (__stdcall* ZR_RD_SELECTCARD)(HANDLE hHandle, Z_KEYNUM* pNum, ZR_CARD_TYPE nType);
	typedef HRESULT (__stdcall* ZR_RD_READULCARD4PAGE)(HANDLE hHandle, INT nPageN, LPVOID pBuf16);
	typedef HRESULT (__stdcall* ZR_RD_WRITEULCARDPAGE)(HANDLE hHandle, INT nPageN, LPCVOID pData4);
	typedef HRESULT (__stdcall* ZR_RD_FINDT57)(HANDLE hHandle, Z_KEYNUM* pNum, PINT pPar, UINT nPsw, ZR_T57_FLAGS nFlags);
	typedef HRESULT (__stdcall* ZR_RD_READT57BLOCK)(HANDLE hHandle, INT nBlockN, LPVOID pBuf4, INT nPar, UINT nPsw, ZR_T57_FLAGS nFlags);
	typedef HRESULT (__stdcall* ZR_RD_WRITET57BLOCK)(HANDLE hHandle, INT nBlockN, LPCVOID pData4, INT nPar, UINT nPsw, ZR_T57_FLAGS nFlags);
	typedef HRESULT (__stdcall* ZR_RD_RESETT57)(HANDLE hHandle);
	typedef HRESULT (__stdcall* ZR_DECODET57CONFIG)(PZR_T57_CONFIG pConfig, LPCVOID pData4);
	typedef HRESULT (__stdcall* ZR_ENCODET57CONFIG)(LPVOID pBuf4, PZR_T57_CONFIG pConfig);
	typedef HRESULT (__stdcall* ZR_DECODET57EMMARINE)(PINT pBitOffs, Z_KEYNUM* pNum, LPCVOID pData, INT nCount);
	typedef HRESULT (__stdcall* ZR_ENCODET57EMMARINE)(LPVOID pBuf8_, INT nBufSize, INT nBitOffs, Z_KEYNUM* pNum);
	typedef HRESULT (__stdcall* ZR_DECODET57HID)(Z_KEYNUM* pNum, LPCVOID pData12, PINT pWiegand);
	typedef HRESULT (__stdcall* ZR_ENCODET57HID)(LPVOID pBuf12, Z_KEYNUM* pNum, INT nWiegand);
	typedef HRESULT (__stdcall* ZR_RD_GETENCODEDCARDNUMBER)(HANDLE hHandle, ZR_CARD_TYPE* pType, Z_KEYNUM* pNum, LPVOID pBuf, INT nBufSize, LPINT pRCount);
	typedef HRESULT (__stdcall* ZR_RD_AUTHORIZESECT)(HANDLE hHandle, INT nBlockN, const ZR_MF_AUTH_KEY* pKey, UINT nFlags/*=0*/);
	typedef HRESULT (__stdcall* ZR_RD_AUTHORIZESECTBYEKEY)(HANDLE hHandle, INT nBlockN, UINT nKeyMask, PINT pRKeyIdx, UINT nFlags/*=0*/);
	typedef HRESULT (__stdcall* ZR_RD_READMFCARDBLOCK)(HANDLE hHandle, INT nBlockN, LPVOID pBuf16, UINT nFlags/*=0*/);
	typedef HRESULT (__stdcall* ZR_RD_WRITEMFCARDBLOCK)(HANDLE hHandle, INT nBlockN, LPCVOID pData16, UINT nFlags/*=0*/);
	typedef HRESULT (__stdcall* ZR_RD_WRITEMFCARDBLOCK4)(HANDLE hHandle, INT nBlockN, LPCVOID pData4);
	typedef HRESULT (__stdcall* ZR_RD_GETINDICATORSTATE)(HANDLE hHandle, ZR_IND_FLAGS* pRed, ZR_IND_FLAGS* pGreen, ZR_IND_FLAGS* pSound);
	typedef HRESULT (__stdcall* ZR_RD_SETINDICATORSTATE)(HANDLE hHandle, ZR_IND_STATE nRed, ZR_IND_STATE nGreen, ZR_IND_STATE nSound);
	typedef HRESULT (__stdcall* ZR_RD_ADDINDICATORFLASH)(HANDLE hHandle, PZR_IND_FLASH pRecs, INT nCount, BOOL fReset, PINT pRCount);
	typedef HRESULT (__stdcall* ZR_RD_BREAKINDICATORFLASH)(HANDLE hHandle, BOOL fAutoMode);
	typedef HRESULT (__stdcall* ZR_RD_GETINDICATORFLASHAVAILABLE)(HANDLE hHandle, PINT pCount);
	typedef HRESULT (__stdcall* ZR_RD_RESET1356)(HANDLE hHandle, WORD nWaitMs);
	typedef HRESULT (__stdcall* ZR_RD_POWEROFF)(HANDLE hHandle);
	typedef HRESULT (__stdcall* ZR_RD_REQUEST)(HANDLE hHandle, BOOL fWakeUp, ZR_CARD_TYPE* pType, PWORD pATQ);
	typedef HRESULT (__stdcall* ZR_RD_HALT)(HANDLE hHandle);
	typedef HRESULT (__stdcall* ZR_RD_A_S)(HANDLE hHandle, Z_KEYNUM* pNum, PBYTE pSAK);
	typedef HRESULT (__stdcall* ZR_RD_R_A_S)(HANDLE hHandle, BOOL fWakeUp, ZR_CARD_TYPE* pType, Z_KEYNUM* pNum, PBYTE pSAK, PWORD pATQ);
	typedef HRESULT (__stdcall* ZR_RD_R_R)(HANDLE hHandle, Z_KEYNUM* pNum, BOOL fWakeUp);
	typedef HRESULT (__stdcall* ZR_RD_RATS)(HANDLE hHandle, LPVOID pBuf, INT nBufSize, LPINT pRCount);
	typedef HRESULT (__stdcall* ZR_RD_AUTH)(HANDLE hHandle, UINT nAddr, PZR_MF_AUTH_KEY pKey, UINT nEKeyMask, PINT pEKeyIdx, UINT nFlags);
	typedef HRESULT (__stdcall* ZR_RD_READ16)(HANDLE hHandle, UINT nAddr, LPVOID pBuf16, UINT nFlags);
	typedef HRESULT (__stdcall* ZR_RD_WRITE16)(HANDLE hHandle, UINT nAddr, LPCVOID pData16, UINT nFlags);
	typedef HRESULT (__stdcall* ZR_RD_WRITE4)(HANDLE hHandle, UINT nAddr, LPCVOID pData4);
	typedef HRESULT (__stdcall* ZR_RD_INCREMENT)(HANDLE hHandle, UINT nAddr, UINT nValue);
	typedef HRESULT (__stdcall* ZR_RD_DECREMENT)(HANDLE hHandle, UINT nAddr, UINT nValue);
	typedef HRESULT (__stdcall* ZR_RD_TRANSFER)(HANDLE hHandle, UINT nAddr);
	typedef HRESULT (__stdcall* ZR_RD_RESTORE)(HANDLE hHandle, UINT nAddr);
	typedef HRESULT (__stdcall* ZR_RD_ISO_MP_4)(HANDLE hHandle, LPCVOID pData, INT nCount, LPVOID pBuf, INT nBufSize, LPINT pRCount);
	typedef HRESULT (__stdcall* ZR_RD_WRITEKEYTOEEPROM)(HANDLE hHandle, UINT nAddr, BOOL fKeyB, PZR_MF_AUTH_KEY pKey);
	typedef HRESULT (__stdcall* ZR_ENCODEMFACCESSBITS)(INT nAreaN, LPVOID pBuf3, UINT nBits);
	typedef HRESULT (__stdcall* ZR_DECODEMFACCESSBITS)(INT nAreaN, PUINT pBits, LPCVOID pData3);
	typedef HRESULT (__stdcall* ZR_M3N_ACTIVATEPOWERKEY)(HANDLE hHandle, BOOL fForce, UINT nTimeMs);
	typedef HRESULT (__stdcall* ZR_M3N_SETOUTPUTS)(HANDLE hHandle, UINT nMask, UINT nOutBits);
	typedef HRESULT (__stdcall* ZR_M3N_GETINPUTS)(HANDLE hHandle, PUINT pFlags);
	typedef HRESULT (__stdcall* ZR_M3N_SETCONFIG)(HANDLE hHandle, PZR_M3N_CONFIG pConfig);
	typedef HRESULT (__stdcall* ZR_M3N_GETCONFIG)(HANDLE hHandle, PZR_M3N_CONFIG pConfig);
	typedef HRESULT (__stdcall* ZR_M3N_SETSECURITY)(HANDLE hHandle, INT nBlockN, UINT nKeyMask, BOOL fKeyB);
	typedef HRESULT (__stdcall* ZR_M3N_GETSECURITY)(HANDLE hHandle, PINT pBlockN, PUINT pKeyMask, PBOOL pKeyB);
	typedef HRESULT (__stdcall* ZR_M3N_SETCLOCK)(HANDLE hHandle, LPSYSTEMTIME pTime);
	typedef HRESULT (__stdcall* ZR_M3N_GETCLOCK)(HANDLE hHandle, LPSYSTEMTIME pTime);
	typedef HRESULT (__stdcall* ZR_Z2B_SETFORMAT)(HANDLE hHandle, LPCWSTR pszFmt, LPCWSTR pszArg, LPCWSTR pszNoCard, BOOL fSaveEE);
	typedef HRESULT (__stdcall* ZR_Z2B_GETFORMAT)(HANDLE hHandle, LPWSTR szFmtBuf, INT nFmtBufSize, 
		LPWSTR szArgBuf, INT nArgBufSize, LPWSTR szNCBuf, INT nNCBufSize);
	typedef HRESULT (__stdcall* ZR_Z2B_SETPOWERSTATE)(HANDLE hHandle, BOOL fOn);
} // extern "C"

extern ZR_GETVERSION ZR_GetVersion;
extern ZR_INITIALIZE ZR_Initialize;
extern ZR_FINALYZE ZR_Finalyze;
extern ZR_UPDATERDFIRMWARE ZR_UpdateRdFirmware;
extern ZR_RD_OPEN ZR_Rd_Open;
extern ZR_RD_DETTACHPORT ZR_Rd_DettachPort;
extern ZR_RD_GETCONNECTIONSTATUS ZR_Rd_GetConnectionStatus;
extern ZR_RD_GETWAITSETTINGS ZR_Rd_GetWaitSettings;
extern ZR_RD_SETWAITSETTINGS ZR_Rd_SetWaitSettings;
extern ZR_RD_SETCAPTURE ZR_Rd_SetCapture;
extern ZR_RD_RELEASECAPTURE ZR_Rd_ReleaseCapture;
extern ZR_RD_GETINFORMATION ZR_Rd_GetInformation;
extern ZR_RD_UPDATEFIRMWARE ZR_Rd_UpdateFirmware;
extern ZR_RD_SETNOTIFICATION ZR_Rd_SetNotification;
extern ZR_RD_GETNEXTMESSAGE ZR_Rd_GetNextMessage;
extern ZR_RD_SEARCHCARDS ZR_Rd_SearchCards;
extern ZR_RD_FINDNEXTCARD ZR_Rd_FindNextCard;
extern ZR_RD_SELECTCARD ZR_Rd_SelectCard;
extern ZR_RD_READULCARD4PAGE ZR_Rd_ReadULCard4Page;
extern ZR_RD_WRITEULCARDPAGE ZR_Rd_WriteULCardPage;
extern ZR_RD_FINDT57 ZR_Rd_FindT57;
extern ZR_RD_READT57BLOCK ZR_Rd_ReadT57Block;
extern ZR_RD_WRITET57BLOCK ZR_Rd_WriteT57Block;
extern ZR_RD_RESETT57 ZR_Rd_ResetT57;
extern ZR_DECODET57CONFIG ZR_DecodeT57Config;
extern ZR_ENCODET57CONFIG ZR_EncodeT57Config;
extern ZR_DECODET57EMMARINE ZR_DecodeT57EmMarine;
extern ZR_ENCODET57EMMARINE ZR_EncodeT57EmMarine;
extern ZR_DECODET57HID ZR_DecodeT57Hid;
extern ZR_ENCODET57HID ZR_EncodeT57Hid;
extern ZR_RD_GETENCODEDCARDNUMBER ZR_Rd_GetEncodedCardNumber;
extern ZR_RD_AUTHORIZESECT ZR_Rd_AuthorizeSect;
extern ZR_RD_AUTHORIZESECTBYEKEY ZR_Rd_AuthorizeSectByEKey;
extern ZR_RD_READMFCARDBLOCK ZR_Rd_ReadMfCardBlock;
extern ZR_RD_WRITEMFCARDBLOCK ZR_Rd_WriteMfCardBlock;
extern ZR_RD_WRITEMFCARDBLOCK4 ZR_Rd_WriteMfCardBlock4;
extern ZR_RD_GETINDICATORSTATE ZR_Rd_GetIndicatorState;
extern ZR_RD_SETINDICATORSTATE ZR_Rd_SetIndicatorState;
extern ZR_RD_ADDINDICATORFLASH ZR_Rd_AddIndicatorFlash;
extern ZR_RD_BREAKINDICATORFLASH ZR_Rd_BreakIndicatorFlash;
extern ZR_RD_GETINDICATORFLASHAVAILABLE ZR_Rd_GetIndicatorFlashAvailable;
extern ZR_RD_RESET1356 ZR_Rd_Reset1356;
extern ZR_RD_POWEROFF ZR_Rd_PowerOff;
extern ZR_RD_REQUEST ZR_Rd_Request;
extern ZR_RD_HALT ZR_Rd_Halt;
extern ZR_RD_A_S ZR_Rd_A_S;
extern ZR_RD_R_A_S ZR_Rd_R_A_S;
extern ZR_RD_R_R ZR_Rd_R_R;
extern ZR_RD_RATS ZR_Rd_RATS;
extern ZR_RD_AUTH ZR_Rd_Auth;
extern ZR_RD_READ16 ZR_Rd_Read16;
extern ZR_RD_WRITE16 ZR_Rd_Write16;
extern ZR_RD_WRITE4 ZR_Rd_Write4;
extern ZR_RD_INCREMENT ZR_Rd_Increment;
extern ZR_RD_DECREMENT ZR_Rd_Decrement;
extern ZR_RD_TRANSFER ZR_Rd_Transfer;
extern ZR_RD_RESTORE ZR_Rd_Restore;
extern ZR_RD_ISO_MP_4 ZR_Rd_ISO_MP_4;
extern ZR_RD_WRITEKEYTOEEPROM ZR_Rd_WriteKeyToEEPROM;
extern ZR_ENCODEMFACCESSBITS ZR_EncodeMfAccessBits;
extern ZR_DECODEMFACCESSBITS ZR_DecodeMfAccessBits;
extern ZR_M3N_ACTIVATEPOWERKEY ZR_M3n_ActivatePowerKey;
extern ZR_M3N_SETOUTPUTS ZR_M3n_SetOutputs;
extern ZR_M3N_GETINPUTS ZR_M3n_GetInputs;
extern ZR_M3N_SETCONFIG ZR_M3n_SetConfig;
extern ZR_M3N_GETCONFIG ZR_M3n_GetConfig;
extern ZR_M3N_SETSECURITY ZR_M3n_SetSecurity;
extern ZR_M3N_GETSECURITY ZR_M3n_GetSecurity;
extern ZR_M3N_SETCLOCK ZR_M3n_SetClock;
extern ZR_M3N_GETCLOCK ZR_M3n_GetClock;
extern ZR_Z2B_SETFORMAT ZR_Z2b_SetFormat;
extern ZR_Z2B_GETFORMAT ZR_Z2b_GetFormat;
extern ZR_Z2B_SETPOWERSTATE ZR_Z2b_SetPowerState;

extern HMODULE	hZrLib;
extern INT nZrLoadCount;

#endif // !ZREADER_LINKONREQUEST

#ifndef ZREADER_EXPORTS
inline BOOL IsZReaderLoaded()
{
#ifdef ZREADER_LINKONREQUEST
	return hZrLib != 0;
#else
	return TRUE;
#endif // ZREADER_LINKONREQUEST
}

HRESULT LoadZReader(LPCTSTR pszPath=NULL);
VOID UnloadZReader();
#endif // !ZREADER_EXPORTS

#define ZR_CloseHandle	ZP_CloseHandle

// Перечислить все подключенные считыватели
#define ZR_GetPortInfoList(pHandle, pCount) (HRESULT)ZP_GetPortInfoList(pHandle, pCount, ZR_DEVTYPE_READERS)
inline HRESULT ZR_SearchDevices(PHANDLE pHandle, PZP_SEARCH_PARAMS pParams, BOOL fSerial=TRUE, BOOL fIP=TRUE)
{
	if (fSerial)
		pParams->nDevMask |= ZR_DEVTYPE_READERS;
	if (fIP)
		pParams->nIpDevMask |= ZR_IPDEVTYPE_CVTS;
	return (HRESULT)ZP_SearchDevices(pHandle, pParams);
};
inline HRESULT ZR_FindNextDevice(HANDLE hHandle, PZR_RD_INFO pInfo, 
	PZP_PORT_INFO pPortArr, INT nArrLen, PINT pPortCount, UINT nTimeout=INFINITE)
{
	pInfo->cbSize = sizeof(_ZR_RD_INFO);
	return ZP_FindNextDevice(hHandle, pInfo, pPortArr, nArrLen, pPortCount, nTimeout);
}
// Настраивает уведомления
inline HRESULT ZR_SetNotification(PHANDLE pHandle, PZP_DD_NOTIFY_SETTINGS pSettings)
{ pSettings->nSDevTypes |= ZR_DEVTYPE_READERS; return (HRESULT)ZP_DD_SetNotification(pHandle, pSettings); }
#define ZR_GetNextMessage	ZP_DD_GetNextMessage


// Устаревшие константы, типы и функции
//#define ZR_EnumSerialPorts(pEnumProc, pUserData) (HRESULT)ZP_EnumSerialPorts(0x19, pEnumProc, pUserData)
#define ZP_SF_UPDATE	1	// Обновить список сейчас
#define ZP_SF_USEVCOM	2	// По возможности использовать Com-порт
inline HRESULT ZR_EnumReaders(PZP_PORT_ADDR pPorts, INT nPCount, 
	ZR_ENUMDRDPROC pEnumProc, PVOID pUserData, PZP_WAIT_SETTINGS pWait=NULL, UINT nFlags=ZP_SF_UPDATE)
{
	//return (HRESULT)ZP_EnumSerialDevices(ZR_DEVTYPE_READERS, pPorts, nPCount, (ZP_ENUMDEVICEPROC)pEnumProc, pUserData, pWait, nFlags); 
	HANDLE hSearch;
	_ZP_SEARCH_PARAMS rParams;
	_ZR_RD_INFO rDI;
	_ZP_PORT_INFO rPI;
	int nPortCount;
	ZeroMemory(&rParams, sizeof(rParams));
	rParams.nDevMask = ZR_DEVTYPE_READERS;
	if (nFlags & ZP_SF_USEVCOM)
		rParams.nFlags |= ZP_SF_USECOM;
	rParams.pPorts	= pPorts;
	rParams.nPCount	= nPCount;
	rParams.pWait	= pWait;
	HRESULT hr = ZP_SearchDevices(&hSearch, &rParams);
	if (FAILED(hr))
		return hr;
	__try
	{
		rDI.cbSize = sizeof(rDI);
		while ((hr = ZP_FindNextDevice(hSearch, &rDI, &rPI, 1, &nPortCount, INFINITE)) == S_OK)
		{
			for (int i = 0; i < nPortCount; i++)
				if (!pEnumProc(&rDI, &rPI, pUserData))
					return ZP_S_CANCELLED;
			rDI.cbSize = sizeof(rDI);
		}
	}
	__finally
	{
		ZP_CloseHandle(hSearch);
	}
	return hr;
}
inline HRESULT ZR_FindReader(PZP_PORT_ADDR pPorts, INT nPCount, 
	PZR_RD_INFO pInfo, PZP_PORT_INFO pPort, PZP_WAIT_SETTINGS pWait=NULL, UINT nFlags=ZP_SF_UPDATE)
{
	HANDLE hSearch;
	_ZP_SEARCH_PARAMS rParams;
	_ZR_RD_INFO rDI;
	_ZP_PORT_INFO aPIs[2];
	int nPortCount;
	ZeroMemory(&rParams, sizeof(rParams));
	rParams.nDevMask = ZR_DEVTYPE_READERS;
	if (nFlags & ZP_SF_USEVCOM)
		rParams.nFlags |= ZP_SF_USECOM;
	rParams.pPorts	= pPorts;
	rParams.nPCount	= nPCount;
	rParams.pWait	= pWait;
	HRESULT hr = ZP_SearchDevices(&hSearch, &rParams);
	if (FAILED(hr))
		return hr;
	__try
	{
		rDI.cbSize = sizeof(rDI);
		ZeroMemory(aPIs, sizeof(aPIs));
		hr = ZP_FindNextDevice(hSearch, &rDI, aPIs, _countof(aPIs), &nPortCount, INFINITE);
		if (FAILED(hr))
			return hr;
		if (hr == S_OK)
		{
			*pInfo = rDI;
			*pPort = aPIs[0];
		}
	}
	__finally
	{
		ZP_CloseHandle(hSearch);
	}
	return hr;
}
inline HRESULT ZR_Rd_EnumMessages(HANDLE hHandle, ZP_NOTIFYPROC pEnumProc, PVOID pUserData)
{
	HRESULT hr;
	UINT nMsg;
	LPARAM nMsgParam;
	while ((hr = ZR_Rd_GetNextMessage(hHandle, &nMsg, &nMsgParam)) == S_OK)
		pEnumProc(nMsg, nMsgParam, pUserData);
	if (hr == ZP_S_NOTFOUND)
		hr = S_OK;
	return hr;
}
inline HRESULT ZR_Rd_FindCard(HANDLE hHandle, ZR_CARD_TYPE* pType, Z_KEYNUM* pNum)
{
	HRESULT hr = ZR_Rd_SearchCards(hHandle, 1, 0);
	if (hr == S_OK)
	{
		_ZR_CARD_INFO rInfo;
		if ((hr = ZR_Rd_FindNextCard(hHandle, &rInfo)) == S_OK)
		{
			*pType = rInfo.nType;
			memcpy(pNum, &rInfo.nNum, sizeof(*pNum));
			ZR_Rd_FindNextCard(hHandle, NULL); // освобождаем память
		}
	}
	return hr;
}
inline HRESULT ZR_Rd_EnumCards(HANDLE hHandle, ZR_ENUMCARDSPROC pEnumProc, PVOID pUserData)
{
	HRESULT hr = ZR_Rd_SearchCards(hHandle, ZR_SEARCH_MAXCARD, 0);
	if (hr == S_OK)
	{
		_ZR_CARD_INFO rInfo;
		while ((hr = ZR_Rd_FindNextCard(hHandle, &rInfo)) == S_OK)
		{
			if (!pEnumProc(&rInfo, pUserData))
			{
				hr = ZP_S_CANCELLED;
				break;
			}
		}
		ZR_Rd_FindNextCard(hHandle, NULL); // освобождаем память
	}
	return hr;
}


typedef HRESULT ZR_STATUS;	// результат выполнения функции DLL

#pragma deprecated(ZR_SUCCESS)
#define ZR_SUCCESS						S_OK
#pragma deprecated(ZR_E_CANCELLED)
#define ZR_E_CANCELLED					ZP_S_CANCELLED
#pragma deprecated(ZR_E_NOT_FOUND)
#define ZR_E_NOT_FOUND					ZP_S_NOTFOUND
#pragma deprecated(ZR_E_INVALID_PARAM)
#define ZR_E_INVALID_PARAM				E_INVALIDARG
#pragma deprecated(ZR_E_OPEN_NOT_EXIST)
#define ZR_E_OPEN_NOT_EXIST				ZP_E_OPENNOTEXIST
#pragma deprecated(ZR_E_OPEN_ACCESS)
#define ZR_E_OPEN_ACCESS				ZP_E_OPENACCESS
#pragma deprecated(ZR_E_OPEN_PORT)
#define ZR_E_OPEN_PORT					ZP_E_OPENPORT
#pragma deprecated(ZR_E_PORT_IO_ERROR)
#define ZR_E_PORT_IO_ERROR				ZP_E_PORTIO
#pragma deprecated(ZR_E_PORT_SETUP)
#define ZR_E_PORT_SETUP					ZP_E_PORTSETUP
#pragma deprecated(ZR_E_LOAD_FTD2XX)
#define ZR_E_LOAD_FTD2XX				ZP_E_LOADFTD2XX
#pragma deprecated(ZR_E_INIT_SOCKET)
#define ZR_E_INIT_SOCKET				ZP_E_SOCKET
#pragma deprecated(ZR_E_SERVERCLOSE)
#define ZR_E_SERVERCLOSE				ZP_E_SERVERCLOSE
#pragma deprecated(ZR_E_NOT_ENOUGH_MEMORY)
#define ZR_E_NOT_ENOUGH_MEMORY			E_OUTOFMEMORY
#pragma deprecated(ZR_E_UNSUPPORT)
#define ZR_E_UNSUPPORT					E_NOINTERFACE
#pragma deprecated(ZR_E_NOT_INITALIZED)
#define ZR_E_NOT_INITALIZED				ZP_E_NOTINITALIZED
#pragma deprecated(ZR_E_CREATE_EVENT)
#define ZR_E_CREATE_EVENT				E_FAIL
#pragma deprecated(ZR_E_INSUFFICIENT_BUFFER)
#define ZR_E_INSUFFICIENT_BUFFER		ZP_E_INSUFFICIENTBUFFER
#pragma deprecated(ZR_E_NO_ANSWER)
#define ZR_E_NO_ANSWER					ZR_E_NOANSWER
#pragma deprecated(ZR_E_BAD_ANSWER)
#define ZR_E_BAD_ANSWER					ZR_E_BADANSWER
#pragma deprecated(ZR_E_CARD_NOT_SELECT)
#define ZR_E_CARD_NOT_SELECT			ZR_E_CARDNOTSELECT
#pragma deprecated(ZR_E_NO_CARD)
#define ZR_E_NO_CARD					ZR_E_NOCARD
#pragma deprecated(ZR_E_WRONG_ZPORT_VERSION)
#define ZR_E_WRONG_ZPORT_VERSION		ZR_E_WRONGZPORT
#pragma deprecated(ZR_E_RD_OTHER)
#define ZR_E_RD_OTHER					ZR_E_RDERROR
#pragma deprecated(ZR_E_CARD_ACCESS)
#define ZR_E_CARD_ACCESS				ZR_E_CARDACCESS
#pragma deprecated(ZR_E_PAGE_LOCK)
#define ZR_E_PAGE_LOCK					ZR_E_PAGELOCK
#pragma deprecated(ZR_E_NO_T57)
#define ZR_E_NO_T57						ZR_E_NOT57
#pragma deprecated(ZR_E_NO_WRITE_T57)
#define ZR_E_NO_WRITE_T57				ZR_E_NOWRITET57
#pragma deprecated(ZR_E_INV_MODEL)
#define ZR_E_INV_MODEL					ZR_E_INVMODEL
#pragma deprecated(ZR_E_INV_BOOTFILE)
#define ZR_E_INV_BOOTFILE				ZR_E_INVBOOTFILE
#pragma deprecated(ZR_E_BUFFER_OVERFLOWN)
#define ZR_E_BUFFER_OVERFLOWN			ZR_E_BUFFEROVERFLOWN
#pragma deprecated(ZR_E_KEY_NOT_FOUND)
#define ZR_E_KEY_NOT_FOUND				ZR_E_KEYNOTFOUND
#pragma deprecated(ZR_E_MIF_FCS)
#define ZR_E_MIF_FCS					ZR_E_RDERROR
#pragma deprecated(ZR_E_MIF_INV_CMD)
#define ZR_E_MIF_INV_CMD				ZR_E_RDERROR
#pragma deprecated(ZR_E_MIF_INV_PAR)
#define ZR_E_MIF_INV_PAR				ZR_E_RDERROR
#pragma deprecated(ZR_E_MIF_RES)
#define ZR_E_MIF_RES					ZR_E_RESEXHAUSTED
#pragma deprecated(ZR_E_MIF_RD_DOWN)
#define ZR_E_MIF_RD_DOWN				ZR_E_RDERROR
#pragma deprecated(ZR_E_MIF_NO_CARD)
#define ZR_E_MIF_NO_CARD				ZR_E_NOCARD
#pragma deprecated(ZR_E_MIF_CD_ANSWER)
#define ZR_E_MIF_CD_ANSWER				ZR_E_RDERROR
#pragma deprecated(ZR_E_MIF_AUTH)
#define ZR_E_MIF_AUTH					ZR_E_AUTHORIZE
#pragma deprecated(ZR_E_MIF_CD_NACK)
#define ZR_E_MIF_CD_NACK				ZR_E_CARDNACK
#pragma deprecated(ZR_E_MIF_PREV_CMD_NC)
#define ZR_E_MIF_PREV_CMD_NC			ZR_E_PREVNOTCOMLETED
#pragma deprecated(ZR_E_OTHER)
#define ZR_E_OTHER						E_FAIL

#pragma deprecated(ZR_RNF_PLACE_CARD)
#define ZR_RNF_PLACE_CARD				ZR_RNF_EXIST_CARD
#pragma deprecated(ZR_RNF_WND_SYNC)
#define ZR_RNF_WND_SYNC					0x4000
#pragma deprecated(ZR_RNF_ONLY_NOTIFY)
#define ZR_RNF_ONLY_NOTIFY				0x8000
#pragma deprecated(ZR_IF_ERROR_LOG)
#define ZR_IF_ERROR_LOG					ZR_IF_LOG

#pragma deprecated(ZR_EnumMessages)
#define ZR_EnumMessages	ZP_EnumMessages
#pragma deprecated(ZR_Rd_Close)
#define ZR_Rd_Close ZR_CloseHandle
#pragma deprecated(ZR_Rd_ProcessMessages)
#define ZR_Rd_ProcessMessages ZR_Rd_EnumMessages
