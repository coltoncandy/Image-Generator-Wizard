#pragma once

#include <QWidget>
#include "ui_targetselector.h"

class TargetSelector : public QWidget {
	Q_OBJECT

public:
	TargetSelector(QWidget* parent = Q_NULLPTR);
	~TargetSelector();
	void setImage(QImage*);
	QImage* getImage();

private:
	Ui::targetselector ui;

	QImage* initialImage;
};
