
#include "imagewizard.h"
#include "batchchoice.h"

BatchChoice::BatchChoice(QWidget* parent) : WizardPage(parent) {
	ui.setupUi(this);

	// Add styling images to buttons
	QPushButton* singleBtn = findChild<QPushButton*>("singleBtn");
	QPushButton* batchBtn = findChild<QPushButton*>("batchBtn");
	batchBtn->setCursor(QCursor(Qt::PointingHandCursor));
	singleBtn->setCursor(QCursor(Qt::PointingHandCursor));
	QString imageHover = QDir::homePath() + "/source/repos/image-generator/icons/imageHover.png";
	QString image = QDir::homePath() + "/source/repos/image-generator/icons/image.png";
	QString singleStyleSheet = "QPushButton#singleBtn{ image: url(" + image + "); width: 85px; height: 32px; background-repeat: no-repeat; border-left: 10px transparent; border-right: 10px transparent; border-top: 3px transparent; border-bottom: 3px transparent; } QPushButton:hover#singleBtn{ image: url(" + imageHover + "); background-repeat: no-repeat; }";
	QString bimageHover = QDir::homePath() + "/source/repos/image-generator/icons/batchImageHover.png";
	QString bimage = QDir::homePath() + "/source/repos/image-generator/icons/batchImage.png";
	QString batchStyleSheet = "QPushButton#batchBtn{ image: url(" + bimage + "); width: 85px; height: 32px; background-repeat: no-repeat; border-left: 10px transparent; border-right: 10px transparent; border-top: 3px transparent; border-bottom: 3px transparent; } QPushButton:hover#batchBtn{ image: url(" + bimageHover + "); background-repeat: no-repeat; }";

	batchBtn->setStyleSheet(singleStyleSheet);
	singleBtn->setStyleSheet(batchStyleSheet);
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