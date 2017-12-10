#include "stdafx.h"
#include "SearchDevice.h"
#include "CustomQLable.h"
#include "ReaderListlVisualSDK.h"

ReaderListlVisualSDK::ReaderListlVisualSDK(QWidget *parent)
	: QMainWindow(parent)
{	
	ui.setupUi(this);
	
	t_searhDevice = static_cast<std::shared_ptr<QThread>>(new QThread());
	SearchDevice *searchDevice = new SearchDevice(t_searhDevice);
	searchDevice->moveToThread(&*t_searhDevice);

	connect(ui.DeviceList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(doubleClick(QListWidgetItem*)));
	connect(searchDevice, SIGNAL(messageFromReader(QString)), this, SLOT(writeToEventList(QString)));
	connect(ui.InputText, SIGNAL(doubleClicked()), this, SLOT(on_InputText_doubleclicked()));
	connect(this, SIGNAL(openConnection(QString)), searchDevice, SLOT(openConnection(QString)));

	//addList((*(new QStringList) << "hello"));
	t_searhDevice->start();

}

void ReaderListlVisualSDK::addList(QStringList string) {
	removeList();
	ui.DeviceList->addItems(string);
}

void ReaderListlVisualSDK::doubleClick(QListWidgetItem* item) {
	auto temp = item->text();
	emit openConnection(temp);
}

void ReaderListlVisualSDK::removeList() {
	ui.DeviceList->clear();
}

void ReaderListlVisualSDK::on_InputText_doubleclicked() {
	QClipboard* clipboard = QGuiApplication::clipboard();
	QString originalText = clipboard->text();
	clipboard->setText("hello");
}

void ReaderListlVisualSDK::writeToEvenList(QString message) {
	ui.EventList->textCursor().insertText(message + '\r');
}