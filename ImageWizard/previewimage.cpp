#include "previewimage.h"
#include "wizardpage.h"
//#include "imagewizard.h"
#include <QLabel>
#include <QMessageBox>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>


PreviewImage::PreviewImage(const QString& title, QWidget* parent)
	: WizardPage(parent){
	ui.setupUi(this);

	processingTitle = "Processing";
	batchTitle = "Here are your generated images";
	singleImageTitle = "Here is your generated image";

	titleLabel = findChild<QLabel*>("title");
	imgLabel = findChild<QLabel*>("imgLabel");
	nextImageButton = findChild<QPushButton*>("nextImageButton");
	saveButton = findChild<QPushButton*>("saveButton");
	processButton = findChild<QPushButton*>("processButton");

	QObject::connect(saveButton, &QPushButton::pressed, this, &PreviewImage::saveImage);
	QObject::connect(nextImageButton, &QPushButton::pressed, this, &PreviewImage::nextImage);
	QObject::connect(processButton, &QPushButton::pressed, this, &PreviewImage::processAgain);
	saveButton->hide();
	nextImageButton->hide();
	processButton->hide();

	batchFlag = false;
	imageIndex = 0;
	imageNum = 0;
}

void PreviewImage::loadImage() {
	if(imageIndex >= imageNum) {
		saveButton->hide();
		return;
	}

	try {
		image = QImage((const unsigned char*) (imageMats[imageIndex].data), imageMats[imageIndex].cols, imageMats[imageIndex].rows, QImage::Format_BGR888);
		saveButton->setEnabled(true);
	}
	catch(...) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", "Failed loading processed image.");
		saveButton->hide();
	}

	scaleImage(imgLabel->size());
}

void PreviewImage::scaleImage(const QSize& size) {
	try {
		QPixmap p = QPixmap::fromImage(image);
		imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
	}
	catch(...) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", "Failed loading scaled image.");
	}
}

void PreviewImage::reset() {
	saveButton->hide();
	nextImageButton->hide();
	processButton->hide();
}

void PreviewImage::resizeEvent(QResizeEvent* e) {
	QWidget::resizeEvent(e);
	scaleImage(imgLabel->size());
}

void PreviewImage::pageSwitched(int imageNum, const std::string& initialPath, const std::string& targetPath, const std::string& backgroundPath, const std::string& destination, bool batchFlag) {
	if(imageMats.size() > 0)
		imageMats.clear();
	imgLabel->clear();
	this->imageNum = imageNum;
	this->initialPath = initialPath;
	this->targetPath = targetPath;
	this->destination = destination;
	this->backgroundPath = backgroundPath;
	this->batchFlag = batchFlag;

	if(batchFlag) {
		processButton->setText("Generate New Batch");
		batchProcess();
	}
	else {
		processButton->setText("Generate New Image");
		process();
	}

	loadImage();
}

void PreviewImage::saveImage() {
	std::string imagePath = createUniqueImageId(destination);
	imwrite(imagePath, imageMats[imageIndex]);
	saveButton->setEnabled(false);
}

void PreviewImage::nextImage() {
	++imageIndex;
	loadImage();
	if(imageIndex == imageNum - 1) {
		nextImageButton->hide();
		processButton->show();
	//	getWizard()->showRestart();
	}

}

void PreviewImage::processAgain() {
	imgLabel->clear();
	imageMats.clear();
	if(batchFlag)
		batchProcess();
	else
		process();
}

void PreviewImage::batchProcess() {
	saveButton->hide();
	nextImageButton->hide();
	processButton->hide();
//	getWizard()->hideRestart();
	titleLabel->setText(processingTitle);
	std::string* backgroundImages = nullptr;
	QGuiApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
	QCoreApplication::processEvents();
	try {
		getRandomImages(imageNum, backgroundPath, backgroundImages);
		if(backgroundImages) {
			AlgoManager::AlgoManager::batchProcess(imageNum, initialPath, targetPath, backgroundImages, imageMats);
			QGuiApplication::restoreOverrideCursor();
			imageNum = imageMats.size();
			if(imageNum > 0) {
				saveButton->show();
				if(imageNum > 1)
					nextImageButton->show();
				imageIndex = 0;
				loadImage();
			}
		}
	}
	catch(std::string ex) {
		QGuiApplication::restoreOverrideCursor();
		QMessageBox messageBox;
		messageBox.warning(0, "Error", ex.c_str());
	}
	titleLabel->setText(batchTitle);
}

void PreviewImage::process() {
	saveButton->hide();
	nextImageButton->hide();
	processButton->hide();
	titleLabel->setText(processingTitle);
	QGuiApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
	QCoreApplication::processEvents();
	try {
		imageMats.push_back(AlgoManager::AlgoManager::process(initialPath, targetPath, backgroundPath));
		loadImage();
	}
	catch(std::string ex) {
		QGuiApplication::restoreOverrideCursor();
		QMessageBox messageBox;
		messageBox.warning(0, "Error", ex.c_str());
	}
	saveButton->show();
	processButton->show();
//	getWizard()->showRestart();
	QGuiApplication::restoreOverrideCursor();
	titleLabel->setText(singleImageTitle);
}
