#include "targetselector.h"
#include <QLabel>
#include <QImage>

TargetSelector::TargetSelector(ImageInfo* target, QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);

	this->target = target;
}

TargetSelector::~TargetSelector() {
}

void TargetSelector::updateImage() {
	QLabel* imgLabel = findChild<QLabel*>("imgLabel");
	imgLabel->setPixmap(QPixmap::fromImage(*(this->target->image)));
}

//open up png file as png in C++
//create mouse events to get coordinates... and give in ratio of total size to png editor in C++
