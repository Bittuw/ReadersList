#include "stdafx.h"
#include "SearchDevice.h"
#include "OpenConnection.h"
#include "CustomQLable.h"
#include "GlobalObjects.h"
#include "ReaderListlVisualSDK.h"

ReaderListlVisualSDK::ReaderListlVisualSDK(QWidget *parent)
	: QMainWindow(parent)
{	
	ui.setupUi(this);
	
	t_searhDevice = static_cast<std::shared_ptr<QThread>>(new QThread());
	SearchDevice *searchDevice = new SearchDevice(t_searhDevice);
	searchDevice->moveToThread(&*t_searhDevice);

	t_openConnection = static_cast<std::shared_ptr<QThread>>(new QThread());
	OpenConnection *openConnection = new OpenConnection();
	openConnection->moveToThread(&*t_openConnection);
	
	//t_openConnection = static_cast<std::shared_ptr<QThread>>(new QThread());

	connect(ui.DeviceList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(doubleClick(QListWidgetItem*)));
	connect(ui.InputText, SIGNAL(doubleClicked()), this, SLOT(on_InputText_doubleclicked()));

	//////////
	connect(
		searchDevice, &SearchDevice::message, 
		this, &ReaderListlVisualSDK::writeToEvenList
	);
	connect(
		searchDevice, &SearchDevice::setDeviceList,
		this, &ReaderListlVisualSDK::addList
	);
	connect(
		searchDevice, &SearchDevice::message,
		this, &ReaderListlVisualSDK::writeToEvenList
	);
	//////////

	//////////
	connect(
		openConnection, &OpenConnection::message, 
		this, &ReaderListlVisualSDK::writeToEvenList
	);
	connect(
		openConnection, &OpenConnection::writeToInputText,
		this, &ReaderListlVisualSDK::writeToInputText
	);
	connect(
		this, &ReaderListlVisualSDK::openConnection,
		openConnection, &OpenConnection::openConnection
	);
	//////////

	t_searhDevice->start();
	t_openConnection->start();
}

void ReaderListlVisualSDK::addList(QStringList string) {
	removeList();
	ui.DeviceList->addItems(string);
}

void ReaderListlVisualSDK::doubleClick(QListWidgetItem* item) {
	SetEvent(*newReader);
	auto temp = item->text();
	emit openConnection(temp);
}

void ReaderListlVisualSDK::removeList() {
	ui.DeviceList->clear();
}

void ReaderListlVisualSDK::on_InputText_doubleclicked() {
	QClipboard* clipboard = QGuiApplication::clipboard();
	QString originalText = clipboard->text();
	clipboard->setText(ui.InputText->text());
}

void ReaderListlVisualSDK::writeToEvenList(QString message) {
	if (ui.EventList->blockCount() >= 100)
		ui.EventList->clear();
	ui.EventList->textCursor().insertText(message + '\r');
	ui.EventList->verticalScrollBar()->setValue(ui.EventList->verticalScrollBar()->maximum() - 6);
}

void ReaderListlVisualSDK::writeToInputText(QString number)
{
	ui.InputText->clear();
	ui.InputText->setText(number);
}

ReaderListlVisualSDK::~ReaderListlVisualSDK() {
	SetEvent(*_globalExit);

	if (t_searhDevice) {
		t_searhDevice->quit();
		t_searhDevice->wait();
	}
	if (t_openConnection) {
		t_openConnection->quit();
		t_openConnection->wait();
	}
}