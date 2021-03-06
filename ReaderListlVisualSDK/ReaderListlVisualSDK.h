#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ReaderListlVisualSDK.h"

class ReaderListlVisualSDK : public QMainWindow
{
	Q_OBJECT

public:
	ReaderListlVisualSDK(QWidget *parent = Q_NULLPTR);
	~ReaderListlVisualSDK();

signals:
	void openConnection(QString);

public slots :

	void addList(QStringList); // ���������� � ������
	void removeList(); // �������� �� ������
	void doubleClick(QListWidgetItem*); // ��������� �������� ������� �� ������
	void writeToEvenList(QString);
	void writeToInputText(QString);

private slots:
	void on_InputText_doubleclicked();

private:
	Ui::ReaderListlVisualSDKClass ui;
	std::shared_ptr<QThread> t_searhDevice;
	std::shared_ptr<QThread> t_openConnection;
};
