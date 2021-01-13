#pragma once

#include <QWidget>
#include "ui_targetselector.h"

//added
#include <QLabel>
#include <QMenu>

class TargetSelector : public QWidget
{
	Q_OBJECT

public:
	TargetSelector(QWidget *parent = Q_NULLPTR);
	~TargetSelector();
	void setImage(QImage*);

//added
protected:
	
	void paintEvent(QPaintEvent* e);
	/*
	virtual void mousePressEvent(QMouseEvent *e) override;
	virtual void mouseMoveEvent(QMouseEvent *e) override;
	virtual void mouseReleaseEvent(QMouseEvent* e) override;
	*/

private:
	Ui::targetselector ui;
	
	QImage* initialImage;

	//added 
	QPoint origin;

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
