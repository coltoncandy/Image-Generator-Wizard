#include <QMessageBox>
#include "imagewizard.h"
#include "filechooser.h"
#include "../AlgoManager/algomanager.h" 

ImageWizard::ImageWizard(QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);

	frames = findChild<QStackedWidget*>("frames");

	initial = new ImageInfo;
	target = new ImageInfo;
	background = new ImageInfo;
	destination = new QString; // new path to store

	welcomePage = new WelcomePage("Welcome to Image Generator");
	targetChooser = new ForegroundChooser("Select or drag an image containing the target", initial, "..\\ImageGallery\\Targets\\Drones", "..\\ImageGallery\\Targets\\Cropped_Drones");
	backgroundChooser = new FileChooser("Select or drag a background image", background, "..\\ImageGallery\\Backgrounds");
	targetSelector = new TargetSelector("Select Target", initial, target);
	selectDestination = new SelectDestination("Select Your Destination", destination);
	processingWindow = new ProcessingWindow("It won't be too long ...");
	backgroundRemoval = new BackgroundRemoval("Background Removal Instructions", target);
	previewImage = new PreviewImage("Here is your Processed Image");

	frames->addWidget(welcomePage);
	frames->addWidget(targetChooser);
	frames->addWidget(targetSelector);
	frames->addWidget(backgroundRemoval);
	frames->addWidget(backgroundChooser);
	frames->addWidget(selectDestination);
	frames->addWidget(processingWindow);
	frames->addWidget(previewImage);

	btnPrev = findChild<QPushButton*>("btnPrev");
	btnNext = findChild<QPushButton*>("btnNext");
	btnPrev->setCursor(QCursor(Qt::PointingHandCursor));
	btnNext->setCursor(QCursor(Qt::PointingHandCursor));

	// Add styling for next and previous buttons
	QString rightHover = QDir::homePath() + "/source/repos/image-generator/icons/rightHover.png";
	QString rightDisabled = QDir::homePath() + "/source/repos/image-generator/icons/rightDisabled.png";
	QString right = QDir::homePath() + "/source/repos/image-generator/icons/rightArrow.png";
	QString rightHoverStyleSheet = "QPushButton#btnNext {image: url(" + right + "); width: 85px; height: 32px; border-left: 10px transparent; border-right: 10px transparent; border-top: 3px transparent; border-bottom: 3px transparent;} QPushButton:hover#btnNext {image: url(" + rightHover + "); background - repeat: no - repeat;} QPushButton:disabled#btnNext {image: url(" + rightDisabled + "); background - repeat: no - repeat; }";
	
	QString leftHover = QDir::homePath() + "/source/repos/image-generator/icons/leftHover.png";
	QString leftDisabled = QDir::homePath() + "/source/repos/image-generator/icons/leftDisabled.png";
	QString left = QDir::homePath() + "/source/repos/image-generator/icons/leftArrow.png";
	QString leftHoverStyleSheet = "QPushButton#btnPrev {image: url(" + left + "); width: 85px; height: 32px; border-left: 10px transparent; border-right: 10px transparent; border-top: 3px transparent; border-bottom: 3px transparent;} QPushButton:hover#btnPrev {image: url(" + leftHover + "); background - repeat: no - repeat;} QPushButton:disabled#btnPrev {image: url(" + leftDisabled + "); background - repeat: no - repeat; }";
	
	btnPrev->setStyleSheet(leftHoverStyleSheet);
	btnNext->setStyleSheet(rightHoverStyleSheet); 

	//Hides the previous button on the first page
	btnPrev->hide();
}

ImageWizard::~ImageWizard() {
	delete welcomePage;
	delete targetChooser;
	delete backgroundChooser;
	delete targetSelector;
	delete selectDestination;
	delete processingWindow;
	delete initial;
	delete target;
	delete background;
	delete destination;
	delete backgroundRemoval;
	delete previewImage;
}

void ImageWizard::enableNext() {
	btnNext->setEnabled(true);
}

void ImageWizard::disableNext() {
	btnNext->setEnabled(false);
}

bool ImageWizard::isNextEnabled() {
	return btnNext->isEnabled();
}

void ImageWizard::enablePrev() {
	btnPrev->setEnabled(true);
}

void ImageWizard::disablePrev() {
	btnPrev->setEnabled(false);
}

bool ImageWizard::isPrevEnabled() {
	return btnPrev->isEnabled();
}

//Next page in UI
void ImageWizard::goNext() {
	QGuiApplication::restoreOverrideCursor();

	int cur = frames->currentIndex();

	// By default, the next button is disabled. If the page is already ready, then
	// it be re-enabled. Otherwise, it must be enabled within the page itself when
	// certain actions are performed. See: FileChooser::loadImage

	WizardPage* currentPage = dynamic_cast<WizardPage*>(frames->currentWidget());

	//Restrict the ability to go to the next page if certain conditions haven't been met

	if(!currentPage->isReady())
		return;

	//if we've reached this point, then we've finished uploading/interacting with pictures on our current page and continue to the next page.
	if(cur < frames->count()) {
		if(frames->currentWidget() == targetChooser && targetChooser->skipCrop()) {
			target->image->load(*(initial->path));
			*(target->path) = *(initial->path);
			target->loaded = true;
			frames->setCurrentIndex(frames->indexOf(backgroundChooser));
		}
		else
			frames->setCurrentIndex(++cur);
		currentPage = dynamic_cast<WizardPage*>(frames->currentWidget());
		if(!currentPage->isReady())
			disableNext();
		else
			enableNext();
		//Hides & shows navigation buttons depending on the current widget
		if(cur == 1) {
			btnPrev->show();
		}
		else if(currentPage == processingWindow) {
			btnNext->hide();
			btnPrev->hide();
			QCoreApplication::processEvents();
			QGuiApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
			AlgoManager::AlgoManager::process(initial->path->toStdString(), target->path->toStdString(), background->path->toStdString(), destination->toStdString());		//Send image containing target to grabCut
			QGuiApplication::restoreOverrideCursor();
			previewImage->updateImage(destination);
			frames->setCurrentIndex(++cur);
			currentPage = dynamic_cast<WizardPage*>(frames->currentWidget());
		}
		else if(cur == frames->count()) {
			btnNext->hide();
		}
	}
	QCoreApplication::processEvents();
	QGuiApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
	currentPage->pageSwitched();
	QGuiApplication::restoreOverrideCursor();
}

//Previous page in UI 
void ImageWizard::goPrev() {
	QGuiApplication::restoreOverrideCursor();

	int cur = frames->currentIndex();

	QMessageBox msg;
	msg.setText("Going back will reset the current page.");
	msg.setInformativeText("Are you sure you want to go back?");
	msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msg.setDefaultButton(QMessageBox::Cancel);
	msg.setIcon(QMessageBox::Warning);
	int ret = msg.exec();

	switch(ret) {
	case QMessageBox::Yes:
		// do nothing
		break;
	case QMessageBox::Cancel:
		return;
		break;
	}

	WizardPage* currentPage = dynamic_cast<WizardPage*>(frames->currentWidget());
	currentPage->reset();

	if(cur > 0) {
		if(frames->currentWidget() == backgroundChooser && targetChooser->skipCrop()) {
			frames->setCurrentIndex(frames->indexOf(targetChooser));
			target->reset();
		}
		else
			frames->setCurrentIndex(--cur);

		currentPage = dynamic_cast<WizardPage*>(frames->currentWidget());
		currentPage->pageSwitched();
		if(!currentPage->isReady())
			disableNext();
		else
			enableNext();
		//Hides & shows navigation buttons depending on the current widget
		if(cur == 0) {
			btnPrev->hide();
		}
		if(cur == frames->count() - 2) {
			btnNext->show();
		}
	}
}