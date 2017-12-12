#include "stdafx.h"
#include "Windows.h"
#include "SearchDevice.h"


SearchDevice::SearchDevice(std::shared_ptr<QThread> thread)
{
	connect(this, SLOT(searchDevices()), &*thread, SIGNAL(started()));
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

					str.append((WCHAR)rPI.szName);
					str.append(" ");
					str.append((WCHAR)rPI.szFriendly);

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

		while (true) {
			if (WaitForSingleObject(r_Event, INFINITE)) {
				ResetEvent(r_Event);
				CheckNotifyMessage(h_Notify);
			}
		}

	}
	catch (const std::exception& error) {
		emit message(QString("SEARCH::") + error.what());
	}
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

				str.append((WCHAR)pInfo->rPort.szName);
				str.append(" ");
				str.append((WCHAR)pInfo->rPort.szFriendly);

				readersList.append(str);
			}
		}
		break;
		case ZP_N_REMOVE:
		{
			PZP_DDN_PORT_INFO pInfo = (PZP_DDN_PORT_INFO)nMsgParam;

			QString str;

			str.append((WCHAR)pInfo->rPort.szName);
			str.append(" ");
			str.append((WCHAR)pInfo->rPort.szFriendly);

			if (readersList.contains(str)) {
				readersList.removeOne(str);
			}
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

