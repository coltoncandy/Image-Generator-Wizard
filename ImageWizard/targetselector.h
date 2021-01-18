#pragma once

#include <QWidget>
#include "ui_targetselector.h"
#include <QLabel>
#include <QMenu>
#include <QRubberBand>
#include <QPushButton>

class TargetSelector : public QWidget
{
	Q_OBJECT

public:
	TargetSelector(const QString& title, QWidget *parent = Q_NULLPTR);
	~TargetSelector();
	void setImage(QImage*);


private:
	Ui::targetselector ui;
	
	QImage* initialImage;
	QPoint origin;
	QRubberBand* rubberBand;
	QImage* croppedImage;
	QPushButton *resetButton;

	QPixmap image;
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

	bool selectionStarted;
	QRect selectionRect;
	QMenu contextMenu;

public slots:
	void reset();
};
