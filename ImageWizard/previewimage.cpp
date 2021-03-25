#include "previewimage.h"
#include "wizardpage.h"
#include <QLabel>
#include <QMessageBox>
#include <QDir>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>


PreviewImage::PreviewImage(const QString& title, QWidget* parent)
	: WizardPage(parent){
	ui.setupUi(this);

	processingTitle = "Processing";
	batchTitle = "Here are your generated images";
	singleImageTitle = "Here is your generated image";
	failedProcessingTitle = "Failed to generate image/s";

	titleLabel = findChild<QLabel*>("title");
	imgLabel = findChild<QLabel*>("imgLabel");
	nextImageButton = findChild<QPushButton*>("nextImageButton");
	saveButton = findChild<QPushButton*>("saveButton");
	processButton = findChild<QPushButton*>("processButton");

	//Add styling to buttons
	nextImageButton->setCursor(QCursor(Qt::PointingHandCursor));
	saveButton->setCursor(QCursor(Qt::PointingHandCursor));
	processButton->setCursor(QCursor(Qt::PointingHandCursor));
	QString saveHover = QDir::currentPath() + "/icons/saveHover.png";
	QString save = QDir::currentPath() + "/icons/save.png";
	QString saveDisabled = QDir::currentPath() + "/icons/saveDisabled.png";
	QString saveStyleSheet = "QPushButton#saveButton{ image: url(" + save + "); width: 100px; height: 50px; background-repeat: no-repeat; border-left: 10px transparent; border-right: 10px transparent; border-top: 3px transparent; border-bottom: 3px transparent; } QPushButton:hover#saveButton{ image: url(" + saveHover + "); background-repeat: no-repeat; } QPushButton:disabled#saveButton{ image: url(" + saveDisabled + "); background-repeat: no-repeat; }";
	QString nextImageHover = QDir::currentPath() + "/icons/nextImageHover.png";
	QString nextImage = QDir::currentPath() + "/icons/nextImage.png";
	QString nextImageDisabled = QDir::currentPath() + "/icons/nextImageDisabled.png";
	QString nextImageStyleSheet = "QPushButton#nextImageButton{ image: url(" + nextImage + "); width: 100px; height: 50px; background-repeat: no-repeat; border-left: 10px transparent; border-right: 10px transparent; border-top: 3px transparent; border-bottom: 3px transparent; } QPushButton:hover#nextImageButton{ image: url(" + nextImageHover + "); background-repeat: no-repeat; } QPushButton:disabled#nextImageButton{ image: url(" + nextImageDisabled + "); background-repeat: no-repeat; }";
	QString processHover = QDir::currentPath() + "/icons/generateMoreHover.png";
	QString process = QDir::currentPath() + "/icons/generateMore.png";
	QString processDisabled = QDir::currentPath() + "/icons/generateMoreDisabled.png";
	QString processStyleSheet = "QPushButton#processButton{ image: url(" + process + "); width: 100px; height: 50px; background-repeat: no-repeat; border-left: 10px transparent; border-right: 10px transparent; border-top: 3px transparent; border-bottom: 3px transparent; } QPushButton:hover#processButton{ image: url(" + processHover + "); background-repeat: no-repeat; } QPushButton:disabled#processButton{ image: url(" + processDisabled + "); background-repeat: no-repeat; }";

	processButton->setToolTip("Generate More"); //Add prompt to process button to inform user
	saveButton->setToolTip("Save");
	nextImageButton->setToolTip("Next Image");
	nextImageButton->setStyleSheet(nextImageStyleSheet);
	saveButton->setStyleSheet(saveStyleSheet);
	processButton->setStyleSheet(processStyleSheet);

	QObject::connect(saveButton, &QPushButton::pressed, this, &PreviewImage::saveImage);
	QObject::connect(nextImageButton, &QPushButton::pressed, this, &PreviewImage::nextImage);
	QObject::connect(processButton, &QPushButton::pressed, this, &PreviewImage::processAgain);
	saveButton->hide();
	nextImageButton->hide();
	processButton->hide();

	batchFlag = false;
	imageIndex = 0;
	imageNum = 0;
	batchSize = 0;
}

void PreviewImage::loadImage() {
	if(imageIndex >= imageNum) {
		saveButton->hide();
		return;
	}

	try {
		image = QImage((const unsigned char*) (imageMats[imageIndex].data), imageMats[imageIndex].cols, imageMats[imageIndex].rows, imageMats[imageIndex].step, QImage::Format_BGR888);
		image.bits();
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
	imageIndex = 0;
}

void PreviewImage::resizeEvent(QResizeEvent* e) {
	QWidget::resizeEvent(e);
	scaleImage(imgLabel->size());
}

void PreviewImage::pageSwitched(int imageNum, const std::string& targetPath, const std::string& backgroundPath, const std::string& destination, bool batchFlag) {
	if(imageMats.size() > 0)
		imageMats.clear();
	imgLabel->clear();
	this->imageNum = imageNum;
	batchSize = imageNum;
	this->targetPath = targetPath;
	this->destination = destination;
	this->backgroundPath = backgroundPath;
	this->batchFlag = batchFlag;

	if(batchFlag) {
		//processButton->setText("Generate New Batch");
		batchProcess();
	}
	else {
		//processButton->setText("Generate New Image");
		process();
	}
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
	}

}

void PreviewImage::processAgain() {
	imgLabel->clear();
	imageMats.clear();
	imageIndex = 0;
	if(batchFlag)
		batchProcess();
	else
		process();
}

void PreviewImage::batchProcess() {
	saveButton->hide();
	nextImageButton->hide();
	processButton->hide();
	titleLabel->setText(processingTitle);
	std::string* backgroundImages = nullptr;
	QGuiApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
	QCoreApplication::processEvents();
	try {
		getRandomImages(batchSize, backgroundPath, backgroundImages);
		if(!backgroundImages) {
			std::string errorMessage = "No .png files could be found in the given background directory: ";
			errorMessage.append(backgroundPath);
			throw errorMessage;
		}

		AlgoManager::AlgoManager::batchProcess(batchSize, targetPath, backgroundImages, imageMats);
	}
	catch(std::string ex) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", ex.c_str());
	}

	QGuiApplication::restoreOverrideCursor();
	imageNum = imageMats.size();
	if(imageNum > 0) {
		saveButton->setEnabled(true);
		saveButton->show();
		if(imageNum > 1)
			nextImageButton->show();
		else
			processButton->show();
		imageIndex = 0;
		loadImage();
		titleLabel->setText(batchTitle);
	}
	else {
		titleLabel->setText(failedProcessingTitle);
		saveButton->setEnabled(false);
		saveButton->show();
		processButton->show();
	}
}

void PreviewImage::process() {
	std::string errorMessage;
	saveButton->hide();
	nextImageButton->hide();
	processButton->hide();
	titleLabel->setText(processingTitle);
	QGuiApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
	QCoreApplication::processEvents();
	try {
		try {
			imageMats.push_back(AlgoManager::AlgoManager::process(targetPath, backgroundPath));
		}
		//If failed to load image, create error message with file name
		catch(int errorCode) {
			if(errorCode == 1) {
				errorMessage = "An error occured while accessing the background image (" + backgroundPath + "): No images were generated.";
				throw errorMessage;
			}
			else if(errorCode == -1) {
				errorMessage = "An error occured while accessing the target image (" + targetPath + "): No images were generated." ;
				throw errorMessage;
			}
		}
		//Create a general error message
		catch(...) {
			errorMessage = "An unexpected error occured while generating your images: No images were generated.";
			throw errorMessage;
		}

		loadImage();
		saveButton->setEnabled(true);
		saveButton->show();
		processButton->show();
		QGuiApplication::restoreOverrideCursor();
		titleLabel->setText(singleImageTitle);
	}
	//Diaplay any error messages in a textbox
	catch(std::string ex) {
		QGuiApplication::restoreOverrideCursor();
		QMessageBox messageBox;
		messageBox.warning(0, "Error", ex.c_str());
		titleLabel->setText(failedProcessingTitle);
		saveButton->setEnabled(false);
		saveButton->show();
		processButton->show();
	}
}
