#pragma once

#include <QWidget>
#include "ui_targetselector.h"
#include "imageinfo.h"
#include <QLabel>
#include <QMenu>
#include <QRubberBand>
#include <QPushButton>

class TargetSelector : public QWidget {
	Q_OBJECT

public:
	TargetSelector(const QString& title, ImageInfo* target, QWidget* parent = Q_NULLPTR);
	~TargetSelector();
	void updateImage();

public slots:
	void resizeEvent(QResizeEvent* e);


private:
	Ui::targetselector ui;

	QImage* initialImage;
	QPoint origin;
	QRubberBand* rubberBand;
	QImage* croppedImage;
	QPushButton* resetButton;

	QPixmap image;
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

	bool selectionStarted;
	QRect selectionRect;
	QMenu contextMenu;


	QLabel* imgLabel;
	ImageInfo* target;

	void scaleImage(const QSize& size);

public slots:
	void reset();
};
