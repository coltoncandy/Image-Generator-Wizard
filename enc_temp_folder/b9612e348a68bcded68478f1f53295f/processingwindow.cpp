#include "processingwindow.h"
#include "imagewizard.h"
#include "../AlgoManager/algomanager.h" 
#include "imageinfo.h"
#include <opencv2/core.hpp>


ProcessingWindow::ProcessingWindow(const QString& title, cv::Mat& image, const ImageInfo* const initialImage, const ImageInfo* const targetImage, const ImageInfo* const backgroundImage, QWidget* parent)
	: WizardPage(parent), processingImage(image), initial(initialImage), target(targetImage), background(backgroundImage) {
	ui.setupUi(this);

	QLabel* titleLabel = findChild<QLabel*>("title"); // show title on Qwidget
	titleLabel->setText(title);

	//processingImage = image;
}

ProcessingWindow::~ProcessingWindow() {
}

void ProcessingWindow::pageSwitched() {
	processingImage = AlgoManager::AlgoManager::process(initial->path->toStdString(), target->path->toStdString(), background->path->toStdString());		//Send image containing target to grabCut
}