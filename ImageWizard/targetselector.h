#pragma once

#include <QWidget>
#include "ui_targetselector.h"
#include "imageinfo.h"

class TargetSelector : public QWidget {
	Q_OBJECT

public:
	TargetSelector(ImageInfo* target, QWidget* parent = Q_NULLPTR);
	~TargetSelector();
	void updateImage();

public slots:
	void resizeEvent(QResizeEvent* e);

private:
	Ui::targetselector ui;

	QLabel* imgLabel;
	QImage* initialImage;

	void scaleImage(const QSize& size);
	ImageInfo* target;
};
