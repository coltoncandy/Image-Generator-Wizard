#include "imagewizard.h"
#include "filechooser.h"
#include "../AlgoManager/algomanager.h" 

ImageWizard::ImageWizard(QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);

	frames = findChild<QStackedWidget*>("frames");

	initial = new ImageInfo;
	target = new ImageInfo;
	background = new ImageInfo;
	destination = new QString;

	welcomePage = new WelcomePage("Welcome to Image Generator");
	targetChooser = new FileChooser("Select or drag an image containing the target", initial);
	backgroundChooser = new FileChooser("Select or drag a background image", background);
	targetSelector = new TargetSelector("Select Target", initial, target);
	selectDestination = new SelectDestination("Select Your Destination", destination);
	processingWindow = new ProcessingWindow();

	frames->addWidget(welcomePage);
	frames->addWidget(targetChooser);
	frames->addWidget(targetSelector);
	frames->addWidget(backgroundChooser);
	frames->addWidget(selectDestination);
	frames->addWidget(processingWindow);

	ready = new bool[frames->count()];
	for(int i = 0; i < frames->count(); i++)
		ready[i] = false;

	btnPrev = findChild<QPushButton*>("btnPrev");
	btnNext = findChild<QPushButton*>("btnNext");
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

	delete[] ready;
}

void ImageWizard::enableNext() {
	btnNext->setEnabled(true);
	ready[frames->currentIndex()] = true;
}

void ImageWizard::disableNext() {
	btnNext->setEnabled(false);
	ready[frames->currentIndex()] = false;
}

void ImageWizard::enablePrev() {
	btnPrev->setEnabled(true);
}

void ImageWizard::disablePrev() {
	btnPrev->setEnabled(false);
}

//Next page in UI
void ImageWizard::goNext() {
	int cur = frames->currentIndex();

	//Restrict the ability to go to the next page if certain conditions haven't been met
	if(frames->currentWidget() == targetChooser) {		//target image upload page
		if(!initial->loaded) {
			return;
		}
	}
	else if(frames->currentWidget() == targetSelector) { //target selection/crop page
		if(!target->loaded) {
			return;
		}
		AlgoManager::AlgoManager::grabCutWrapper(target->path->toStdString());		//NOTE: Needs to be changed to target->path after SC-35 is complete 
		target->image->load(*target->path);											//Update target struct for processed image written to target->path 
	}
	else if(frames->currentWidget() == backgroundChooser) { //background image upload page
		if(!background->loaded) {
			return;
		}
	}
	else if(frames->currentWidget() == selectDestination) { //background image upload page
		if(!selectDestination->isReady()) {
			return;
		}
	}
	else if(frames->currentWidget() == processingWindow ) {
			AlgoManager::AlgoManager::overlayWrapper(background->path->toStdString(), target->path->toStdString());		//Send image containing target to grabCut

	}

	//if we've reached this point, then we've finished uploading/interacting with pictures on our current page and continue to the next page.
	if(cur < frames->count()) {
		frames->setCurrentIndex(++cur);
		if(!ready[cur])
			disableNext();
		//Hides & shows navigation buttons depending on the current widget
		if(cur == 1) {
			ready[cur-1] = true;
			btnPrev->show();
		}
		if(cur == frames->count()) {
			btnNext->hide();
		}
		if(frames->currentWidget() == targetSelector) {
			*(target->image) = initial->image->copy();
			targetSelector->updateImage();
		}
	}
}

//Previous page in UI 
void ImageWizard::goPrev() {
	int cur = frames->currentIndex();
	if(cur > 0) {
		frames->setCurrentIndex(--cur);
		if(ready[cur])
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