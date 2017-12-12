#include "stdafx.h"
#include "Windows.h"
#include "GlobalObjects.h"
#include "SearchDevice.h"


SearchDevice::SearchDevice(std::shared_ptr<QThread> thread)
{
	connect(
		&*thread, &QThread::started, 
		this, &SearchDevice::searchDevices
	);
}


SearchDevice::~SearchDevice()
{
	ZR_Finalyze();
}

void SearchDevice::searchDevices() {

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
	
	try
	{
		INT nPortCount;
		HANDLE hList;
		if (ZR_GetPortInfoList(&hList, &nPortCount) != S_OK)
			return;
		try
		{
			_ZP_PORT_INFO rPI;
			for (int i = 0; i < nPortCount; i++)
			{
				ZP_GetPortInfo(hList, i, &rPI);

				if (rPI.nType == ZP_PORT_FT) {

					QString str;

					str.append(convertToQString(rPI.szName));
					str.append(" ");
					str.append(convertToQString(rPI.szFriendly));

					readersList.append(str);
				}
			}
		}
		catch (const std::exception& error) {
			emit message(QString("SEARCH::") + error.what());
		}

		ZR_CloseHandle(hList);

		emit setDeviceList(readersList);

		HANDLE r_Event = CreateEvent(NULL, TRUE, FALSE, NULL);
		HANDLE h_Notify = NULL;
		_ZP_DD_NOTIFY_SETTINGS rNS = { 0 };
		rNS.nNMask = ZP_NF_EXIST;
		rNS.hEvent = r_Event;

		if (ZR_SetNotification(&h_Notify, &rNS) != S_OK)
			emit message(QString("SEARCH::Error to ZR_SetNotification: Nothing device will not be added!"));

		std::vector<HANDLE> eventList = { *_globalExit, r_Event };

		while (true) {
			auto event = WaitForMultipleObjects(eventList.size(), eventList.data(), FALSE, INFINITE);

			if (event == WAIT_OBJECT_0 + 0)
				break;

			if (event == WAIT_OBJECT_0 + 1) {
				ResetEvent(r_Event);
				CheckNotifyMessage(h_Notify);
				continue;
			}
			else {
				emit message(QString("SEARCH:: Error of wating object! Restart"));
				continue;
			}
		}

	}
	catch (const std::exception& error) {
		emit message(QString("SEARCH::") + error.what());
	}
	ZR_Finalyze();
}

void SearchDevice::CheckNotifyMessage(HANDLE& h_Notify)
{
	HRESULT hr;
	UINT nMsg;
	LPARAM nMsgParam;
	while ((hr = ZP_DD_GetNextMessage(h_Notify, &nMsg, &nMsgParam)) == S_OK)
	{
		switch (nMsg)
		{
		case ZP_N_INSERT: {
			PZP_DDN_PORT_INFO pInfo = (PZP_DDN_PORT_INFO)nMsgParam;

			if (pInfo->rPort.nType == ZP_PORT_FT) {

				QString str;

				str.append(convertToQString(pInfo->rPort.szName));
				str.append(" ");
				str.append(convertToQString(pInfo->rPort.szFriendly));

				readersList.append(str);

				emit message(QString("SEACRH:: Find device: " + convertToQString(pInfo->rPort.szName)));
			}
		}
		break;
		case ZP_N_REMOVE:
		{
			PZP_DDN_PORT_INFO pInfo = (PZP_DDN_PORT_INFO)nMsgParam;

			QString str;

			str.append(convertToQString(pInfo->rPort.szName));
			str.append(" ");
			str.append(convertToQString(pInfo->rPort.szFriendly));

			if (readersList.contains(str)) {
				readersList.removeOne(str);
			}

			emit message(QString("SEACRH:: Lost device: " + convertToQString(pInfo->rPort.szName)));
		}
		break;
		}
	}
	if (hr == ZP_S_NOTFOUND) {
		hr = S_OK;
		emit setDeviceList(readersList);
	}
	else {
		emit message(QString("SEARCH::Error to get notification from detector. This is problem, but you can not pay attention."));
	}
}

QString SearchDevice::convertToQString(WCHAR *convert)
{
	std::wstring temp(convert);
	std::string str(temp.begin(), temp.end());
	return QString::fromUtf8(str.c_str());
}

