#pragma once
#include <QWidget>

class CustomQLable : public QLabel
{
	Q_OBJECT

public:
	explicit CustomQLable(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
	~CustomQLable();

signals:
	void clicked();
	void doubleClicked();

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);
};

