#pragma once

#include <QtWidgets/QWidget>
#include <QImage>
#include "ui_imagewizard.h"
#include "filechooser.h"
#include "targetselector.h"

class ImageWizard : public QWidget {
	Q_OBJECT

public:
	ImageWizard(QWidget* parent = Q_NULLPTR);
	~ImageWizard();

public slots:
	void goNext();
	void goPrev();

protected:
	QImage* target;
	QImage* background;

private:
	Ui::ImageWizardClass ui;
	QStackedWidget* frames;                 //Holds pages for the UI 
	FileChooser* targetChooser;
	FileChooser* backgroundChooser;
	TargetSelector* targetSelector;

	void setTargetSelectorImage();

};
