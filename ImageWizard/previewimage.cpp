#include "previewimage.h"
#include "wizardpage.h"
#include <QLabel>
#include <QMessageBox>
#include <opencv2/core.hpp>


PreviewImage::PreviewImage(const QString& title, const cv::Mat& processedImage, QWidget* parent)
	: WizardPage(parent), imageMat(processedImage) {
	ui.setupUi(this);

	QLabel* titleLabel = findChild<QLabel*>("title");
	titleLabel->setText(title);

	imgLabel = findChild<QLabel*>("imgLabel");
}

void PreviewImage::loadImage() {
	try {
		image = QImage((const unsigned char*) (imageMat.data), imageMat.cols, imageMat.rows, QImage::Format_BGR888);
	}
	catch(...) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", "Failed loading processed image.");
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
	imgLabel->clear();
}

void PreviewImage::resizeEvent(QResizeEvent* e) {
	QWidget::resizeEvent(e);
	scaleImage(imgLabel->size());
}

void PreviewImage::pageSwitched() {
	loadImage();
}

PreviewImage::~PreviewImage() {
}