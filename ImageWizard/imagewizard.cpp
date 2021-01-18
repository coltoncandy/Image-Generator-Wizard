#include "imagewizard.h"
#include "filechooser.h"
#include "../AlgoManager/algomanager.h"


ImageWizard::ImageWizard(QWidget* parent)
	: QWidget(parent) {
	ui.setupUi(this);

	frames = findChild<QStackedWidget*>("frames");

	targetChooser = new FileChooser("Select or drag an image containing the target");
	backgroundChooser = new FileChooser("Select or drag a background image");
	targetSelector = new TargetSelector();
	welcomePage = new WelcomePage("Welcome to Image Generator");

	frames->addWidget(welcomePage);
	frames->addWidget(targetChooser);
	frames->addWidget(targetSelector);
	frames->addWidget(backgroundChooser);

	btnPrev = findChild<QPushButton*>("btnPrev");
	btnNext = findChild<QPushButton*>("btnNext");
	//Hides the previous button on the first page
	btnPrev->hide();
}
//Next page in UI
void ImageWizard::goNext() {
	int cur = frames->currentIndex();
	//Restrict the ability to go to the next page if certain conditions haven't been met
	if(frames->currentWidget() == targetChooser) { //target image upload page
		if(targetChooser->getImage() == nullptr) {
			return;
		}
	}
	else if(frames->currentWidget() == targetSelector) { //target selection/crop page
		if(targetSelector->getImage() == nullptr) {
			return;
		}
	}
	else if(frames->currentWidget() == backgroundChooser) { //background image upload page
		if(backgroundChooser->getImage() == nullptr) {
			return;
		}
	}

	//if we've reached this point, then we've finished uploading/interacting with pictures on our current page and continue to the next page.
	if(cur < frames->count()) {
		frames->setCurrentIndex(++cur);

		//Hides & shows navigation buttons depending on the current widget
		if(cur == 1) {
			btnPrev->show();
		}
		if(cur == frames->count() - 1) {
			btnNext->hide();
		}

		if(frames->currentWidget() == targetSelector) {
			setTargetSelectorImage();
		}
	}
}
//Previous page in UI 
void ImageWizard::goPrev() {
	//test function for static library... to be deleted after validation
	AlgoManager::AlgoManager::testFunction();

	int cur = frames->currentIndex();
	if(cur > 0) {
		frames->setCurrentIndex(--cur);

		//Hides & shows navigation buttons depending on the current widget
		if(cur == 0){
			btnPrev->hide();
		}
		if(cur == frames->count() - 2) {
			btnNext->show();
		}

		if(frames->currentWidget() == targetSelector) {
			setTargetSelectorImage();
		}
	}
}

void ImageWizard::setTargetSelectorImage() {
	targetSelector->setImage(targetChooser->getImage());
}