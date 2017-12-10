#include "stdafx.h"
#include "ReaderListlVisualSDK.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ReaderListlVisualSDK w;
	w.show();
	return a.exec();
}
