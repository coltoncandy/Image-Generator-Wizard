#pragma once

#include <QWidget>
#include "ui_targetselector.h"
#include "imageinfo.h"
#include <QLabel>
#include <QRubberBand>
#include <QPushButton>

class TargetSelector : public QWidget {
	Q_OBJECT

public:
	TargetSelector(const QString& title, ImageInfo* initial, ImageInfo* target, QWidget* parent = Q_NULLPTR);
	~TargetSelector();
	void updateImage();

public slots:
	void resizeEvent(QResizeEvent* e);
	void reset();


private:
	Ui::targetselector ui;

	QLabel* imgLabel;
	ImageInfo* target;
	ImageInfo* initial;
	QPoint origin;
	QPoint terminal;
	QRubberBand* rubberBand;
	QPushButton* resetButton;

	void removeBorder();
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void scaleImage(const QSize& size);
};
