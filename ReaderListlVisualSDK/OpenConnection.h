#pragma once

//LPCTSTR CardTypeStrs[] = {
//	TEXT("Unknown"),
//	TEXT("EM"),
//	TEXT("HID"),
//	TEXT("IC"),
//	TEXT("UL"),
//	TEXT("1K"),
//	TEXT("4K"),
//	TEXT("DF"),
//	TEXT("PX"),
//	TEXT("Cod433 Fix"),
//	TEXT("Cod433"),
//	TEXT("Dallas"),
//	TEXT("CAME"),
//	TEXT("Plus"),
//	TEXT("Plus 1K"),
//	TEXT("Plus 2K"),
//	TEXT("Plus 4K"),
//	TEXT("Mini")
//};

class OpenConnection : public QObject
{
	Q_OBJECT

public:
	OpenConnection();
	~OpenConnection();

signals:
	void message(QString);
	void writeToInputText(QString);

public slots:
	void openConnection(QString);

private:
	void CheckNotifyMessage();
	QString ZKeyNumToStr(const Z_KEYNUM&, ZR_CARD_TYPE);
	QString convertToQString(WCHAR*);

	HANDLE h_Connection;
	HANDLE h_Event;

	const int maxEventWaitConnection = 3;
};

