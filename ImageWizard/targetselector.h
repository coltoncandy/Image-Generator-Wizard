#pragma once

#include <QWidget>
#include "ui_targetselector.h"

class TargetSelector : public QWidget {
	Q_OBJECT

public:
	TargetSelector(QWidget* parent = Q_NULLPTR);
	~TargetSelector();
	void setImage(QImage*);

public slots:
	void resizeEvent(QResizeEvent* e);

private:
	Ui::targetselector ui;

	QLabel* imgLabel;
	QImage* initialImage;

	void scaleImage(const QSize& size);
};
