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
	instructions->setText("<div><b>Instructions: </b></div><br><div>1. Hold 'ctrl' key and use mouse to draw on areas of background</div><div>2. Hold 'shift' key and use mouse to draw on areas of foreground</div><div>3. Press 'n' to run one iteration of GrabCut</div><div>4. Press 'r' key to start over </div><div>5. Press 'esc' key to continue once satisifed with result</div>");
	cropButton = findChild<QPushButton*>("cropButton");
	targetImage = target;
	enableDisplay = false;
}

BackgroundRemoval::~BackgroundRemoval() {
}

void BackgroundRemoval::pageSwitched() {
	grabCut();
	enableDisplay = true;
}

void BackgroundRemoval::resizeEvent(QResizeEvent* e) {
	QWidget::resizeEvent(e);
	if(enableDisplay) {
		scaleImage(imgLabel->size());
	}
}

void BackgroundRemoval::grabCut() {
	cropButton->setEnabled(false);
	getWizard()->disableNext();
	getWizard()->disablePrev();
	QGuiApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
	bool finished = AlgoManager::AlgoManager::grabCutWrapper(targetImage->path->toStdString());
	QGuiApplication::restoreOverrideCursor();
	getWizard()->enablePrev();
	getWizard()->enableNext();
	cropButton->setEnabled(true);

	targetImage->image->load(*targetImage->path);

	QSize& size = imgLabel->size();
	QPixmap p = QPixmap::fromImage(*(targetImage->image));
	imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
}

void BackgroundRemoval::scaleImage(const QSize& size) {

	QPixmap p = QPixmap::fromImage(*(targetImage->image));
	imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
}

void BackgroundRemoval::editButton() {
	grabCut();
}

void BackgroundRemoval::reset() {
	imgLabel->clear();
	enableDisplay = false;
}