//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//
#define STATUS_SEVERITY_OK               0x0
#define STATUS_SEVERITY_FAIL             0x2


//
// MessageId: ZP_S_CANCELLED
//
// MessageText:
//
// Operation canceled.
//
#define ZP_S_CANCELLED                   ((HRESULT)0x00040201L)

//
// MessageId: ZP_S_NOTFOUND
//
// MessageText:
//
// Not found.
//
#define ZP_S_NOTFOUND                    ((HRESULT)0x00040202L)

//
// MessageId: ZP_E_OPENNOTEXIST
//
// MessageText:
//
// Port not exists.
//
#define ZP_E_OPENNOTEXIST                ((HRESULT)0x80040203L)

//
// MessageId: ZP_E_OPENPORT
//
// MessageText:
//
// Error opening port.
//
#define ZP_E_OPENPORT                    ((HRESULT)0x80040205L)

//
// MessageId: ZP_E_PORTIO
//
// MessageText:
//
// Communication error.
//
#define ZP_E_PORTIO                      ((HRESULT)0x80040206L)

//
// MessageId: ZP_E_PORTSETUP
//
// MessageText:
//
// Error configuring port.
//
#define ZP_E_PORTSETUP                   ((HRESULT)0x80040207L)

//
// MessageId: ZP_E_LOADFTD2XX
//
// MessageText:
//
// Failed to load FTD2XX.DLL.
//
#define ZP_E_LOADFTD2XX                  ((HRESULT)0x80040208L)

//
// MessageId: ZP_E_SOCKET
//
// MessageText:
//
// Socket Error.
//
#define ZP_E_SOCKET                      ((HRESULT)0x80040209L)

//
// MessageId: ZP_E_SERVERCLOSE
//
// MessageText:
//
// Object is closed from the outside.
//
#define ZP_E_SERVERCLOSE                 ((HRESULT)0x8004020AL)

//
// MessageId: ZP_E_NOTINITALIZED
//
// MessageText:
//
// Not initialized function "Initialize".
//
#define ZP_E_NOTINITALIZED               ((HRESULT)0x8004020BL)

//
// MessageId: ZR_E_NOANSWER
//
// MessageText:
//
// No Answer.
//
#define ZR_E_NOANSWER                    ((HRESULT)0x80040302L)

//
// MessageId: ZR_E_BADANSWER
//
// MessageText:
//
// Bad Answer.
//
#define ZR_E_BADANSWER                   ((HRESULT)0x80040303L)

//
// MessageId: ZR_E_CARDNOTSELECT
//
// MessageText:
//
// The card is not selected by the ZR_Rd_SelectCard.
//
#define ZR_E_CARDNOTSELECT               ((HRESULT)0x80040304L)

//
// MessageId: ZR_E_NOCARD
//
// MessageText:
//
// Card not found.
//
#define ZR_E_NOCARD                      ((HRESULT)0x80040305L)

//
// MessageId: ZR_E_WRONGZPORT
//
// MessageText:
//
// Not the correct version of "ZPort.dll"
//
#define ZR_E_WRONGZPORT                  ((HRESULT)0x80040306L)

//
// MessageId: ZR_E_RDERROR
//
// MessageText:
//
// Unknown error reader.
//
#define ZR_E_RDERROR                     ((HRESULT)0x80040307L)

//
// MessageId: ZR_E_CARDACCESS
//
// MessageText:
//
// No access to the card.
//
#define ZR_E_CARDACCESS                  ((HRESULT)0x80040308L)

//
// MessageId: ZR_E_PAGELOCK
//
// MessageText:
//
// Page is locked.
//
#define ZR_E_PAGELOCK                    ((HRESULT)0x80040309L)

//
// MessageId: ZR_E_NOT57
//
// MessageText:
//
// Card not type T5557.
//
#define ZR_E_NOT57                       ((HRESULT)0x8004030AL)

//
// MessageId: ZR_E_NOWRITET57
//
// MessageText:
//
// Unable to write to the T5557 (unit is locked for writing or wrong password).
//
#define ZR_E_NOWRITET57                  ((HRESULT)0x8004030BL)

//
// MessageId: ZR_E_INVMODEL
//
// MessageText:
//
// Mismatch model (with firmware).
//
#define ZR_E_INVMODEL                    ((HRESULT)0x8004030CL)

//
// MessageId: ZR_E_INVBOOTFILE
//
// MessageText:
//
// Invalid data firmware.
//
#define ZR_E_INVBOOTFILE                 ((HRESULT)0x8004030DL)

//
// MessageId: ZR_E_BUFFEROVERFLOWN
//
// MessageText:
//
// Buffer overflow.
//
#define ZR_E_BUFFEROVERFLOWN             ((HRESULT)0x8004030EL)

//
// MessageId: ZR_E_KEYNOTFOUND
//
// MessageText:
//
// Appropriate key is not found.
//
#define ZR_E_KEYNOTFOUND                 ((HRESULT)0x8004030FL)

//
// MessageId: ZR_E_AUTHORIZE
//
// MessageText:
//
// Error authorization cards.
//
#define ZR_E_AUTHORIZE                   ((HRESULT)0x80040310L)

//
// MessageId: ZR_E_CARDNACK
//
// MessageText:
//
// Card refused performed by team.
//
#define ZR_E_CARDNACK                    ((HRESULT)0x80040311L)

//
// MessageId: ZR_E_RESEXHAUSTED
//
// MessageText:
//
// Device resources are exhausted.
//
#define ZR_E_RESEXHAUSTED                ((HRESULT)0x80040312L)

//
// MessageId: ZR_E_PREVNOTCOMLETED
//
// MessageText:
//
// The previous command is not completed.
//
#define ZR_E_PREVNOTCOMLETED             ((HRESULT)0x80040313L)

