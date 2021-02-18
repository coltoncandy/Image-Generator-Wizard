#include "backgroundremoval.h"
#include "imagewizard.h"
#include "../AlgoManager/algomanager.h" 

BackgroundRemoval::BackgroundRemoval(const QString& title, ImageInfo* target, QWidget* parent)
	: WizardPage(parent) {
	ui.setupUi(this);
	imgLabel = findChild<QLabel*>("label");
	QLabel* titleLabel = findChild<QLabel*>("title");
	titleLabel->setText(title);
	QLabel* instructions = findChild<QLabel*>("instructions");
	instructions->setStyleSheet("QLabel { color : white; }");
	instructions->setText("<ul><b>Instructions: </b><br><li>1. Hold 'ctrl' key and use mouse to draw on areas of background</li><li>3. Hold 'shift' key and use mouse to draw on areas of foreground</li><li>4. Press 'n' to run one iteration of GrabCut</li><li>5. Press 'r' key to start over </li><li>6. Press 'esc' key to continue once satisifed with result</li></ul>");
	cropButton = findChild<QPushButton*>("cropButton");
	targetImage = target;
}

BackgroundRemoval::~BackgroundRemoval() {
}

void BackgroundRemoval::disableButton() {
	cropButton->setEnabled(false);
}

void BackgroundRemoval::enableButton() {
	cropButton->setEnabled(true);
}

void BackgroundRemoval::pageSwitched() {

	//display initial cropped image
	imgLabel->clear();
	disableButton();
	//call algomanager grabcut wrapper 
	getWizard()->disableNext();
	getWizard()->disablePrev();
	bool finished = AlgoManager::AlgoManager::grabCutWrapper(targetImage->path->toStdString());		//NOTE: Needs to be changed to target->path after SC-35 is complete 
	getWizard()->enablePrev();
	if(finished) {
		getWizard()->enableNext();
		enableButton();
	}
	else {
		getWizard()->disableNext();
	}
	//AlgoManager::AlgoManager::grabCutWrapper(targetImage->path->toStdString());		//NOTE: Needs to be changed to target->path after SC-35 is complete 

	//load image into imageinfo struct
	targetImage->image->load(*targetImage->path);

	QSize& size = imgLabel->size();
	QPixmap p = QPixmap::fromImage(*(targetImage->image));
	imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));


	//display imageinfo with new display
}

void BackgroundRemoval::editButton() {
	disableButton();
	getWizard()->disableNext();
	getWizard()->disablePrev();
	bool finished = AlgoManager::AlgoManager::grabCutWrapper(targetImage->path->toStdString());
	getWizard()->enablePrev();
	getWizard()->enableNext();
	enableButton();

	targetImage->image->load(*targetImage->path);

	QSize& size = imgLabel->size();
	QPixmap p = QPixmap::fromImage(*(targetImage->image));
	imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
}