#pragma once
class SearchDevice : public QObject
{
	Q_OBJECT

public:
	SearchDevice(std::shared_ptr<QThread>);
	~SearchDevice();

signals:
	void setDeviceList(QStringList);
	void messageFromReader(QString);

public slots:
	void openConnection(QString);

private:
	void searchDevices();
	
	std::set<_ZR_RD_INFO> readersList;
	std::unique_ptr<HANDLE> h_Reader;
};

