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

public slots:
	void openConnection(QString);

private:
	void searchDevices();
	void CheckNotifyMessage(HANDLE&);

	QStringList readersList;
};

