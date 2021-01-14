#pragma once

#include <QWidget>
#include "ui_targetselector.h"
#include <QLabel>
#include <QMenu>
#include <QRubberBand>

class TargetSelector : public QWidget
{
	Q_OBJECT

public:
	TargetSelector(QWidget *parent = Q_NULLPTR);
	~TargetSelector();
	void setImage(QImage*);


private:
	Ui::targetselector ui;
	
	QImage* initialImage;

	//added 
	QPoint origin;
	QRubberBand* rubberBand;
	QImage* croppedImage;

	QPixmap image;
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

	bool selectionStarted;
	QRect selectionRect;
	QMenu contextMenu;
	


	/*
private slots:
	void saveSlot();
	*/
};
