#pragma once

#include <QtWidgets/QWidget>
#include <QImage>
#include "ui_imagewizard.h"
#include "filechooser.h"
#include "targetselector.h"
#include "imageinfo.h"
#include "welcomepage.h"
#include "selectdestination.h"
#include "processingwindow.h"
#include "backgroundremoval.h"

class ImageWizard : public QWidget {
	Q_OBJECT

public:
	ImageWizard(QWidget* parent = Q_NULLPTR);
	~ImageWizard();

	void enableNext();
	void disableNext();
	bool isNextEnabled();

	void enablePrev();
	void disablePrev();
	bool isPrevEnabled();

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
	SelectDestination* selectDestination;
	ProcessingWindow* processingWindow;
	BackgroundRemoval* backgroundRemoval;
	ImageInfo* initial;
	ImageInfo* target;
	ImageInfo* background;
	QString* destination;
};
