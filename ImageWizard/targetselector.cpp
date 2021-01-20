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

void TargetSelector::scaleImage(const QSize& size) {
	QPixmap p = QPixmap::fromImage(*(target->image));
	imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
}

void TargetSelector::resizeEvent(QResizeEvent* e) {
	QWidget::resizeEvent(e);

	/*
	if(!target->loaded)
		return;
	*/

	scaleImage(imgLabel->size());
}

void TargetSelector::updateImage() {
	imgLabel->setPixmap(QPixmap::fromImage(*(this->target->image)));
	scaleImage(imgLabel->size());
}

//open up png file as png in C++
//create mouse events to get coordinates... and give in ratio of total size to png editor in C++
