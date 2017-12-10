#include "stdafx.h"
#include "CustomLabel.h"


CustomLabel::CustomLabel(QWidget* parent, Qt::WindowFlags f) :
	QLabel(parent)
{
}


CustomLabel::~CustomLabel()
{
}

void CustomLabel::mousePressEvent(QMouseEvent* event) {
	emit clicked();
}

void CustomLabel::mouseDoubleClickEvent(QMouseEvent* event) {
	emit doubleClicked();
}