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
#include "previewimage.h"
#include "batchchoice.h"
#include "batchoptions.h"

class ImageWizard : public QWidget {
	Q_OBJECT

public:
	ImageWizard(QWidget* parent = Q_NULLPTR);
	~ImageWizard();

	void hideNext();
	void showNext();
	void enableNext();
	void disableNext();
	bool isNextEnabled();

	void hidePrev();
	void showPrev();
	void enablePrev();
	void disablePrev();
	bool isPrevEnabled();

	void singleMode();
	void batchMode();

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
	BatchChoice* batchChoice;
	BatchOptions* batchOptions;
	FileChooser* backgroundChooser;
	TargetSelector* targetSelector;
	SelectDestination* selectDestination;
	ProcessingWindow* processingWindow;
	BackgroundRemoval* backgroundRemoval;
	PreviewImage* previewImage;
	ImageInfo* initial;
	ImageInfo* target;
	ImageInfo* background;
	QString* destination;
	BatchInfo batchInfo;
};
