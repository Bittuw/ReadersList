#pragma once
class SearchDevice : public QObject
{
	Q_OBJECT

public:
	SearchDevice(std::shared_ptr<QThread>);
	~SearchDevice();

signals:
	void setDeviceList(QStringList);
	void message(QString);

private:
	void searchDevices();
	void CheckNotifyMessage(HANDLE&);
	QString convertToQString(WCHAR*);

	QStringList readersList;
};

