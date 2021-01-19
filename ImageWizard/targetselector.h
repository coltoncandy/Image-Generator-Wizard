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

private:
	Ui::targetselector ui;

	ImageInfo* target;
};
