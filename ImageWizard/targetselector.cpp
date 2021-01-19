#include "targetselector.h"
#include <QImage>
#include <QMouseEvent>
#include <QMainWindow>

TargetSelector::TargetSelector(const QString& title, ImageInfo* target, QWidget* parent) : QWidget(parent), rubberBand(0) {
	ui.setupUi(this);
	
	resetButton = findChild<QPushButton*>("reset");
	QObject::connect(resetButton, &QPushButton::released, this, &TargetSelector::reset);
	QLabel* titleLabel = findChild<QLabel*>("title");
	titleLabel->setText(title);
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

	QPixmap imageCrop(rect.size());
	imageCrop = grab(rubberBand->geometry()); //copy the selected part
	ui.imgLabel->setPixmap(imageCrop); //show "image" in the second QLabel
	
}

// User presses reset button to set page to original configuration
void TargetSelector::reset()
{
	updateImage();
}

