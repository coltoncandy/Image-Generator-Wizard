#include "imagewizard.h"
#include "filechooser.h"
#include "../AlgoManager/algomanager.h"


ImageWizard::ImageWizard(QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);

	frames = findChild<QStackedWidget*>("frames");

	initial = new ImageInfo;
	target = new ImageInfo;
	background = new ImageInfo;

	targetChooser = new FileChooser("Select or drag an image containing the target", initial);
	backgroundChooser = new FileChooser("Select or drag a background image", background);
	targetSelector = new TargetSelector(target);

	frames->addWidget(targetChooser);
	frames->addWidget(targetSelector);
	frames->addWidget(backgroundChooser);
}

ImageWizard::~ImageWizard() {
	delete targetChooser;
	delete backgroundChooser;
	delete targetSelector;
}

//Next page in UI
void ImageWizard::goNext() {
	int cur = frames->currentIndex();
	if(cur < frames->count()) {
		frames->setCurrentIndex(++cur);
		if(frames->currentWidget() == targetSelector) {
			*(target->image) = initial->image->copy();
			targetSelector->updateImage();
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
	}
}