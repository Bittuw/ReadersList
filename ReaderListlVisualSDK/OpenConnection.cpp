#include "stdafx.h"
#include "OpenConnection.h"
#include "GlobalObjects.h"
#include <QTimer>

OpenConnection::OpenConnection()
{
}


OpenConnection::~OpenConnection()
{
	if (h_Connection != NULL)
		ZR_CloseHandle(h_Connection);
	if (h_Event != NULL)
		CloseHandle(h_Event);
}

void OpenConnection::openConnection(QString device) {
	ResetEvent(*newReader);

	// TODO подключится
	if (ZR_Initialize(ZP_IF_NO_MSG_LOOP) != S_OK) {
		QMessageBox msgBox;
		msgBox.setText("Warning!");
		msgBox.setInformativeText("Error to initial ZR_Reader SDK!");
		msgBox.setStandardButtons(QMessageBox::Close);
		int ret = msgBox.exec();

		switch (ret) {
		case QMessageBox::Close:
			exit(0);
		default:
			exit(0);
		}
	}


	_ZR_RD_INFO rRdInf;
	_ZR_RD_OPEN_PARAMS rOpen;
	ZeroMemory(&rRdInf, sizeof(rRdInf));
	ZeroMemory(&rOpen, sizeof(rOpen));
	rOpen.nType = ZP_PORT_COM;

	wchar_t* array;
	auto temp = device.right(device.size() - device.indexOf("COM")); // TODO проверить
	temp.append("\0");
	array = new wchar_t[(size_t)temp.size() + 1];
	temp.toWCharArray(array);
	array[temp.size()] = 0;
	rOpen.pszName = array;

	try {

		if (ZR_Rd_Open(&h_Connection, &rOpen, &rRdInf) != S_OK) {
			throw std::logic_error("CONNECTION:: can not ZR_Rd_Open reader!");
		}

		emit message(QString("CONNECTION:: connected"));

		_ZR_RD_NOTIFY_SETTINGS rNS = { 0 };
		h_Event = CreateEvent(NULL, TRUE, FALSE, NULL);
		rNS.nNMask = ZR_RNF_EXIST_CARD;
		rNS.hEvent = h_Event;

		if (ZR_Rd_SetNotification(h_Connection, &rNS) != S_OK) {
			throw std::logic_error("CONNECTION:: can not ZR_Rd_SetNotification reader!");
		}
		
		std::vector<HANDLE> eventList = { *_globalExit, *newReader,  h_Event };

		auto tempWait = maxEventWaitConnection;

		while (true) {
			auto event = WaitForMultipleObjects(eventList.size(), eventList.data(), FALSE, 5000);

			if (event == WAIT_OBJECT_0 + 0) {
				break;
			}
			if (event == WAIT_OBJECT_0 + 1) {
				ResetEvent(eventList.at(1));
				break;
			}
			if (event == WAIT_OBJECT_0 + 2) {
				CheckNotifyMessage();
				continue;
			}
			if (event == WAIT_TIMEOUT) {
				ZP_CONNECTION_STATUS status;
				if (ZR_Rd_GetConnectionStatus(h_Connection, &status) != S_OK) {
					emit message(QString("CONNECTION:: can not ZR_Rd_GetConnectionStatus!"));
				}

				if (status == ZP_CS_DISCONNECTED || tempWait == 0) {
					emit message(QString("CONNECTION:: connection lost!"));
					break;
				}
				if (status == ZP_CS_RESTORATION) {
					emit message(QString("CONNECTION:: connection may be lost!"));
					tempWait--;
				}
			}
			else {
				throw std::runtime_error("CONNECTION:: Error of wating object for notification!");
			}
		}
		
	}
	catch (const std::exception& error) {
		emit message(QString(error.what()));
		if(h_Connection != NULL)
			ZR_CloseHandle(h_Connection);
		if (h_Event != NULL)
			CloseHandle(h_Event);
	}

	if (h_Connection != NULL)
		ZR_CloseHandle(h_Connection);
	if (h_Event != NULL)
		CloseHandle(h_Event);

	emit message(QString("CONNECTION:: disconnected"));
	ZR_Finalyze();
}

void OpenConnection::CheckNotifyMessage() {
	HRESULT hr;
	UINT nMsg;
	LPARAM nMsgParam;
	while ((hr = ZR_Rd_GetNextMessage(h_Connection, &nMsg, &nMsgParam)) == S_OK)
	{
		switch (nMsg)
		{
		case ZR_RN_CARD_INSERT:
		{
			PZR_CARD_INFO pInfo = (PZR_CARD_INFO)nMsgParam;
			auto key = ZKeyNumToStr(pInfo->nNum, pInfo->nType);
			emit message(QString("CONNECTION:: Insert key: ") + key);
			emit writeToInputText(key);
			
		}
		break;
		case ZR_RN_CARD_REMOVE:
		{
			PZR_CARD_INFO pInfo = (PZR_CARD_INFO)nMsgParam;
			auto key = ZKeyNumToStr(pInfo->nNum, pInfo->nType);
			emit message(QString("CONNECTION:: Remove key: ") + key);
		}
		break;
		}
	}
	if (hr == ZP_S_NOTFOUND)
		hr = S_OK;
	
}

QString OpenConnection::ZKeyNumToStr(const Z_KEYNUM& rNum, ZR_CARD_TYPE nType)
{
	std::wstring s;
	switch (nType)
	{
	case ZR_CD_EM:
	{
		INT i, j;
			s.resize(rNum[0] * 2 + 1);
			j = 0;
			for (i = rNum[0]; i > 0; i--) {
				int n = _stprintf_s(&s[j], s.size() - j, TEXT("%.2X"), rNum[i]);
				j += 2;
			}
			s.resize(rNum[0] * 2);
	}
	break;
	case ZR_CD_HID:
	{
		DWORD nFacility = 0;
		int nFCnt = std::min(rNum[0] - 2, 4);
		memcpy_s(&nFacility, sizeof(nFacility), &rNum[3], nFCnt);
		s.resize(32);
		int n = swprintf_s((LPTSTR)s.data(), s.size(), TEXT("[%.*X] %d"), nFCnt * 2, nFacility, *(PWORD)&rNum[1]);
		s.resize(std::max(n, 0));
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
	std::string temp(s.begin(), s.end());
return QString::fromUtf8(temp.c_str());
}

QString OpenConnection::convertToQString(WCHAR *convert)
{
	std::wstring temp(convert);
	std::string str(temp.begin(), temp.end());
	return QString::fromUtf8(str.c_str());
}