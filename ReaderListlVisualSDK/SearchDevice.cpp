#include "stdafx.h"
#include "Windows.h"
#include "SearchDevice.h"


SearchDevice::SearchDevice(std::shared_ptr<QThread> thread)
{
	if (ZR_Initialize(ZP_IF_NO_MSG_LOOP) != S_OK)
		return;
	connect(this, SLOT(searchDevices()), &*thread, SIGNAL(started()));
}


SearchDevice::~SearchDevice()
{
	ZR_Finalyze();
}

void SearchDevice::searchDevices() {

	if (ZR_Initialize(ZP_IF_NO_MSG_LOOP) != S_OK)
		return;
	try
	{
		INT nPortCount;
		HANDLE hList;
		QStringList list;
		if (ZR_GetPortInfoList(&hList, &nPortCount) != S_OK)
			return;
		try
		{
			_ZP_PORT_INFO rPI;
			for (int i = 0; i < nPortCount; i++)
			{
				ZP_GetPortInfo(hList, i, &rPI);

				QString str;
				str.append((WCHAR)rPI.nType);
				str.append((WCHAR)rPI.szName);
				str.append(" ");
				str.append((WCHAR)rPI.szFriendly);

				list.append(str);
			}
		}
		catch (const std::exception& error) {
			emit messageFromReader(error.what());
		}

		ZR_CloseHandle(hList);

		emit setDeviceList(list);

	}
	catch (const std::exception& error) {
		emit messageFromReader(error.what());
	}
}

void SearchDevice::openConnection(QString device) { // Подключится к ридеру и слушать + отдавать сообщения
	
}