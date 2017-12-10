#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ReaderListlVisualSDK.h"

class ReaderListlVisualSDK : public QMainWindow
{
	Q_OBJECT

public:
	ReaderListlVisualSDK(QWidget *parent = Q_NULLPTR);

private:
	Ui::ReaderListlVisualSDKClass ui;
};
