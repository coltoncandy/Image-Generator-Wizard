#include "targetselector.h"
#include <QLabel>
#include <QImage>

TargetSelector::TargetSelector(ImageInfo* target, QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);

	imgLabel = findChild<QLabel*>("imgLabel");

	this->target = target;
}

TargetSelector::~TargetSelector() {
}

void TargetSelector::setImage(QImage* image) {
	if(image)
		initialImage = image;

	scaleImage(imgLabel->size());
}

void TargetSelector::scaleImage(const QSize& size) {
	QPixmap p = QPixmap::fromImage(*(this->initialImage));
	imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
}

void TargetSelector::resizeEvent(QResizeEvent* e) {
	QWidget::resizeEvent(e);

	if(!this->initialImage)
		return;

	scaleImage(imgLabel->size());
void TargetSelector::updateImage() {
	QLabel* imgLabel = findChild<QLabel*>("imgLabel");
	imgLabel->setPixmap(QPixmap::fromImage(*(this->target->image)));
}

//open up png file as png in C++
//create mouse events to get coordinates... and give in ratio of total size to png editor in C++
