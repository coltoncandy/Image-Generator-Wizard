#pragma once

#include <QtWidgets/QWidget>
#include "ui_imagewizard.h"
#include "filechooser.h"
#include "targetselector.h"
#include "welcomepage.h"

class ImageWizard : public QWidget {
	Q_OBJECT

public:
	ImageWizard(QWidget* parent = Q_NULLPTR);

public slots:
	void goNext();
	void goPrev();

private:
	Ui::ImageWizardClass ui;
	QStackedWidget* frames;                 //Holds pages for the UI 
	QPushButton* btnPrev;
	QPushButton* btnNext;
	WelcomePage* welcomePage;
	FileChooser* targetChooser;
	FileChooser* backgroundChooser;
	TargetSelector* targetSelector;

	void setTargetSelectorImage();
};
