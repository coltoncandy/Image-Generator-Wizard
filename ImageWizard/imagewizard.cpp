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

	frames->addWidget(targetChooser);
	frames->addWidget(targetSelector);
	frames->addWidget(backgroundChooser);
}
//Next page in UI
void ImageWizard::goNext() {
	int cur = frames->currentIndex();
	if(cur < frames->count()) {
		frames->setCurrentIndex(++cur);
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
		if(frames->currentWidget() == targetSelector) {
			setTargetSelectorImage();
		}
	}
}

void ImageWizard::setTargetSelectorImage() {
	targetSelector->setImage(targetChooser->getImage());
}