#pragma once
class CustomLabel : public QLabel
{
	Q_OBJECT

public:
	explicit CustomLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
	~CustomLabel();

signals:
	void clicked();
	void doubleClicked();

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);

};

