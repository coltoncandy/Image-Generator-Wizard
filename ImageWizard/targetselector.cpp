#include "targetselector.h"
#include <QLabel>
#include <QImage>

TargetSelector::TargetSelector(QWidget* parent) : QWidget(parent) {
	this->initialImage = NULL;
	ui.setupUi(this);
}

TargetSelector::~TargetSelector() {
}

void TargetSelector::setImage(QImage* image) {
	if(image)
		initialImage = image;
	QLabel* imgLabel = findChild<QLabel*>("imgLabel");
	imgLabel->setPixmap(QPixmap::fromImage(*(this->initialImage)));
}

//open up png file as png in C++
//create mouse events to get coordinates... and give in ratio of total size to png editor in C++
