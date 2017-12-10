#include "stdafx.h"
#include "CustomQLable.h"


CustomQLable::CustomQLable(QWidget* parent, Qt::WindowFlags f):
	QLabel(parent){}


CustomQLable::~CustomQLable()
{
}

void CustomQLable::mousePressEvent(QMouseEvent* event) {
	emit clicked();
}

void CustomQLable::mouseDoubleClickEvent(QMouseEvent* event) {
	emit doubleClicked();
}