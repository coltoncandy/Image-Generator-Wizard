#include "targetselector.h"
#include <QImage>
#include <QMouseEvent>


TargetSelector::TargetSelector(QWidget *parent)
	: QWidget(parent), rubberBand(0)
{
	ui.setupUi(this);

}

TargetSelector::~TargetSelector()
{
}

void TargetSelector::setImage(QImage* image) {
	initialImage = image;
	QLabel* imgLabel = findChild<QLabel*>("imgLabel");  
	imgLabel->setPixmap(QPixmap::fromImage(*(this->initialImage)));
}

//open up png file as png in C++
//create mouse events to get coordinates... and give in ratio of total size to png editor in C++


// Initial selection box click 
void TargetSelector::mousePressEvent(QMouseEvent* e)
{
	origin = e->pos(); //origin of first click is set to position 
	if (!rubberBand)
		rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
	rubberBand->setGeometry(QRect(origin, QSize()));
	rubberBand->show();
}

// Allows selection box to follow users movement
void TargetSelector::mouseMoveEvent(QMouseEvent* e)
{
	rubberBand->setGeometry(QRect(origin, e->pos()).normalized());
}

// User releases mouse. Image is then cropped then shown
void TargetSelector::mouseReleaseEvent(QMouseEvent* event)
{
	rubberBand->hide();
	QRect rect; //selection rectangle
	rect.setTopLeft(origin); //top of rectangle is set to first click
	rect.setBottomRight(event->pos()); //bottom of rectangle is set to where the user chose to release mouse

	QPixmap image(rect.size());
	image = grab(rubberBand->geometry()); //copy the selected part
	ui.imgLabel->setPixmap(image); //show "image" in the second QLabel
	
}

