#include "previewimage.h"
#include "wizardpage.h"
#include <QLabel>
#include <QMessageBox>

PreviewImage::PreviewImage(const QString& title, QWidget *parent)
	: WizardPage(parent)
{
	ui.setupUi(this);

	QLabel* titleLabel = findChild<QLabel*>("title");
	titleLabel->setText(title);

	imgLabel = findChild<QLabel*>("imgLabel");

	processedImage = new ImageInfo;

}

void PreviewImage::updateImage(const QString* path) {
	loadImage(path);
}

void PreviewImage::loadImage(const QString* path) {
	try {
		*processedImage->path = *path + "/processed.png";		//fix hard code here
		processedImage->image->load(*processedImage->path);
		processedImage->loaded = true;
	}
	catch(...) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", "Failed loading processed image.");
	}

	scaleImage(imgLabel->size());
}

void PreviewImage::scaleImage(const QSize& size) {
	if(!processedImage->loaded)
		return;

	QPixmap p = QPixmap::fromImage(*(processedImage->image));
	imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
}

void PreviewImage::reset() {
	imgLabel->clear();
	processedImage->reset();
}

void PreviewImage::resizeEvent(QResizeEvent* e) {
	QWidget::resizeEvent(e);
	scaleImage(imgLabel->size());
}

PreviewImage::~PreviewImage()
{
	delete processedImage;
}