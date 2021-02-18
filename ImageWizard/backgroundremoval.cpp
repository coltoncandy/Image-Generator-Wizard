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
	QFont f("Arial", 9);
	instructions->setFont(f);
	instructions->setStyleSheet("QLabel { color : white; }");
	instructions->setText("<li><b>Instructions: </b></li><br><li>1. Hold 'ctrl' key and use mouse to draw on areas of background</li><li>2. Hold 'shift' key and use mouse to draw on areas of foreground</li><li>3. Press 'n' to run one iteration of GrabCut</li><li>4. Press 'r' key to start over </li><li>5. Press 'esc' key to continue once satisifed with result</li>");
	cropButton = findChild<QPushButton*>("cropButton");
	targetImage = target;
	enableDisplay = false;
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

	disableButton();
	getWizard()->disableNext();
	getWizard()->disablePrev();

	bool finished = AlgoManager::AlgoManager::grabCutWrapper(targetImage->path->toStdString());	

	enableDisplay = true;
	getWizard()->enablePrev();
	getWizard()->enableNext();
	enableButton();

	targetImage->image->load(*targetImage->path);

	QSize& size = imgLabel->size();
	QPixmap p = QPixmap::fromImage(*(targetImage->image));
	imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
}

void BackgroundRemoval::resizeEvent(QResizeEvent* e) {
	QWidget::resizeEvent(e);
	if(enableDisplay) {
		scaleImage(imgLabel->size());
	}
}

void BackgroundRemoval::scaleImage(const QSize& size) {

	QPixmap p = QPixmap::fromImage(*(targetImage->image));
	imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
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

void BackgroundRemoval::reset() {
	imgLabel->clear();
	enableDisplay = false;
}