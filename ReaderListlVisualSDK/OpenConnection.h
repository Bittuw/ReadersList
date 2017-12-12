#pragma once
class OpenConnection
{
public:
	OpenConnection(std::shared_ptr<QThread>);
	~OpenConnection();

public slots:
	void openConnection(QString);

private:
	HANDLE h_Connection = NULL;
};

