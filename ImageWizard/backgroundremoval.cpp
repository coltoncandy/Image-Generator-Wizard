#include "backgroundremoval.h"
#include "imagewizard.h"
#include "../AlgoManager/algomanager.h" 


BackgroundRemoval::BackgroundRemoval(const QString& title, ImageInfo* target, QWidget* parent)
	: WizardPage(parent) {
	ui.setupUi(this);
	QLabel* titleLabel = findChild<QLabel*>("title");
	titleLabel->setText(title);
	QLabel* instructions = findChild<QLabel*>("instructions");
	instructions->setText("<b>Instructions: </b><br><ul><li>1. Hold 'ctrl' key and use mouse to draw on areas of background</li><li>3. Hold 'shift' key and use mouse to draw on areas of foreground</li><li>4. Press 'n' to run one iteration of GrabCut</li><li>5. Press 'r' key to start over </li><li>6. Press 'esc' key to continue once satisifed with result</li></ul>");
	
	targetImage = target;
}

BackgroundRemoval::~BackgroundRemoval() {
}

void BackgroundRemoval::pageSwitched() {
	//load initial cropped image

	//call algomanager grabcut wrapper 
	AlgoManager::AlgoManager::grabCutWrapper(targetImage->path->toStdString());		//NOTE: Needs to be changed to target->path after SC-35 is complete 

	//load image into imageinfo struct
	targetImage->image->load(*targetImage->path);

	//display imageinfo with new display
}