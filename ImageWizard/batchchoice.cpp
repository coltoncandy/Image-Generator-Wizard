
#include "imagewizard.h"
#include "batchchoice.h"

BatchChoice::BatchChoice(QWidget* parent) : WizardPage(parent) {
	ui.setupUi(this);

	// Add styling images to buttons
	QPushButton* singleBtn = findChild<QPushButton*>("singleBtn");
	QPushButton* batchBtn = findChild<QPushButton*>("batchBtn");
	batchBtn->setCursor(QCursor(Qt::PointingHandCursor));
	singleBtn->setCursor(QCursor(Qt::PointingHandCursor));
	QString imageHover = QDir::currentPath() + "/icons/imageHover.png";
	QString image = QDir::currentPath() + "/icons/image.png";
	QString singleStyleSheet = "QPushButton#singleBtn{ font: 14pt Microsoft Sans Serif; color:rgb(250, 250, 250); image: url(" + image + "); width: 300px; height: 220px; background-repeat: no-repeat; border-left: 10px transparent; border-right: 10px transparent; border-top: 3px transparent; border-bottom: 3px transparent; } QPushButton:hover#singleBtn{ image: url(" + imageHover + "); background-repeat: no-repeat; }";
	QString bimageHover = QDir::currentPath() + "/icons/batchImageHover.png";
	QString bimage = QDir::currentPath() + "/icons/batchImage.png";
	QString batchStyleSheet = "QPushButton#batchBtn{ font: 14pt Microsoft Sans Serif; color:rgb(250, 250, 250); image: url(" + bimage + "); width: 220px; height: 160px; background-repeat: no-repeat; border-left: 10px transparent; border-right: 10px transparent; border-top: 3px transparent; border-bottom: 3px transparent; } QPushButton:hover#batchBtn{ image: url(" + bimageHover + "); background-repeat: no-repeat; }";

	
	batchBtn->setToolTip("Batch");
	singleBtn->setToolTip("Single");
	batchBtn->setStyleSheet(batchStyleSheet);
	singleBtn->setStyleSheet(singleStyleSheet);
}

BatchChoice::~BatchChoice() {}


void BatchChoice::pageSwitched() {
	getWizard()->hideNext();
}

void BatchChoice::chooseSingle() {
	getWizard()->singleMode();
}

void BatchChoice::chooseBatch() {
	getWizard()->batchMode();
}