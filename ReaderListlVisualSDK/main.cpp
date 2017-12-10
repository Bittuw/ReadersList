#include "stdafx.h"
#include "ReaderListlVisualSDK.h"
#include <QtWidgets/QApplication>
//#include "Utils.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ReaderListlVisualSDK w;

	//CZReaderLoader oZRL;

	//if (!oZRL.IsLoaded())
	//{
	//	qDebug() << "Load ZReader error";
	//	getchar();
	//	return 0;
	//}

	w.show();
	return a.exec();
}
