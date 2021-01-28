#include "targetselector.h"
#include <QImage>
#include <QMouseEvent>
#include <QMainWindow>
#include <QDir>
#include <QMessageBox>

TargetSelector::TargetSelector(const QString& title, ImageInfo* initial, ImageInfo* target, QWidget* parent) : QWidget(parent), rubberBand(0) {
	ui.setupUi(this);

	resetButton = findChild<QPushButton*>("reset");
	QObject::connect(resetButton, &QPushButton::released, this, &TargetSelector::reset);
	QLabel* titleLabel = findChild<QLabel*>("title");
	titleLabel->setText(title);
	imgLabel = findChild<QLabel*>("imgLabel");

	this->target = target;
	this->initial = initial;
}

TargetSelector::~TargetSelector() {
}

void TargetSelector::scaleImage(const QSize& size) {
	QPixmap p = QPixmap::fromImage(*(initial->image));
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
	imgLabel->setPixmap(QPixmap::fromImage(*(this->initial->image)));
	scaleImage(imgLabel->size());
}

//open up png file as png in C++
//create mouse events to get coordinates... and give in ratio of total size to png editor in C++


// Initial selection box click 
void TargetSelector::mousePressEvent(QMouseEvent* e) {
	// create a try and catch. if the target->loaded true then dont let the press occur
	try {
		if(target->loaded)
			throw "Image already cropped. Press retry button to try again";
		// if user clicks in area that is not in the point where photo exists
		// throw value "Must crop only the photo"

	}
	catch(const char * warn) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", warn);
	}
	origin = e->pos(); //origin of first click is set to position 
	if(!rubberBand)
		rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
	rubberBand->setGeometry(QRect(origin, QSize()));
	rubberBand->show();
}

// Allows selection box to follow users movement
void TargetSelector::mouseMoveEvent(QMouseEvent* e) {
	terminal = e->pos();
	rubberBand->setGeometry(QRect(origin, terminal).normalized());
}

// User releases mouse. Image is then cropped then shown
void TargetSelector::mouseReleaseEvent(QMouseEvent* event) {
	rubberBand->hide();

	removeBorder();

	QSize size = this->size();

	//int height = target->image->height();
	//int width = target->image->width();

	int widgetHeight = size.rheight();
	int widgetWidth = size.rwidth();

	//int initialImageHeight = target->image->height();
	//int initialImageWidth = target->image->width();
	int initialImageHeight = initial->image->height();
	int initialImageWidth = initial->image->width();

	int scaledImageHeight = 0;
	int scaledImageWidth = 0;

	int xMin = 0;
	int yMin = 0;
	int xMax = 0;
	int yMax = 0;

	if((double) initialImageHeight / (double) initialImageWidth > ((double) 307 / (double) 619)) {
		scaledImageHeight = widgetHeight - 72;
		scaledImageWidth = scaledImageHeight * initialImageWidth / initialImageHeight;

		if(origin.ry() < terminal.ry()) {
			yMin = origin.ry() - 33;
			yMax = terminal.ry() - 33;
		}
		else {
			yMin = terminal.ry() - 33;
			yMax = origin.ry() - 33;
		}

		if(origin.rx() < terminal.rx()) {
			xMin = origin.rx() - 9 - (widgetWidth / 2 - scaledImageWidth / 2 - 9);
			xMax = terminal.rx() - 9 - (widgetWidth / 2 - scaledImageWidth / 2 - 9);
		}
		else {
			xMin = terminal.rx() - 9 - (widgetWidth / 2 - scaledImageWidth / 2 - 9);
			xMax = origin.rx() - 9 - (widgetWidth / 2 - scaledImageWidth / 2 - 9);
		}
	}
	else {
		scaledImageWidth = widgetWidth - 18;
		scaledImageHeight = scaledImageWidth * initialImageHeight / initialImageWidth;

		if(origin.rx() < terminal.rx()) {
			xMin = origin.rx() - 9;
			xMax = terminal.rx() - 9;
		}
		else {
			xMin = terminal.rx() - 9;
			xMax = origin.rx() - 9;
		}

		if(origin.ry() < terminal.ry()) {
			yMin = origin.ry() - 33 - (((widgetHeight - 72) / 2) - (scaledImageHeight / 2));
			yMax = terminal.ry() - 33 - (((widgetHeight - 72) / 2) - (scaledImageHeight / 2));
		}
		else {
			yMin = terminal.ry() - 33 - (((widgetHeight - 72) / 2) - (scaledImageHeight / 2));
			yMax = origin.ry() - 33 - (((widgetHeight - 72) / 2) - (scaledImageHeight / 2));
		}
	}

	int newX = initialImageWidth * xMin / scaledImageWidth;
	int newY = initialImageHeight * yMin / scaledImageHeight;
	int newHeight = initialImageHeight * yMax / scaledImageHeight - newY;
	int newWidth = initialImageWidth * xMax / scaledImageWidth - newX;
	/*
	*(target->image) = initial->image->copy(newX, newY, newWidth, newHeight);
	target->image = initial->image->copy(newX, newY, newWidth, newHeight);
	target->image = target->image->copy(newX, newY, newWidth, newHeight;*/

	//QRect rect; //selection rectangle
	//rect.setTopLeft(origin); //top of rectangle is set to first click
	//rect.setBottomRight(event->pos()); //bottom of rectangle is set to where the user chose to release mouse
	//QPixmap imageCrop(rect.size());

	//rubberBand->setGeometry(QRect(origin, QSize()));
	//rubberBand->setGeometry(QRect(origin, terminal).normalized());

	//imageCrop = grab(rubberBand->geometry()); //copy the selected part
	//ui.imgLabel->setPixmap(imageCrop); //show "image" in the second QLabel
	*(target->image) = initial->image->copy(newX, newY, newWidth, newHeight); // copying qimage to qimage
	//ui.imgLabel-> do image and pixmap show
	QPixmap imageCrop = QPixmap::fromImage(*(target->image));
	ui.imgLabel->setPixmap(imageCrop); //show "image" in the second QLabel
	*(target->path) = QDir::currentPath() + "/cropped.png"; //set path to current directory
	// pop up window error if save isn't successful. resets widget
	if(!target->image->save(*(target->path), "PNG", 100)) //increased quality to 100
	{
		QMessageBox messageBox;
		messageBox.warning(0, "Error", "Save failed, select target again");
		reset();
		return;
	}
	target->loaded = true;
}

void TargetSelector::removeBorder() {
	// the border is always constant whatever resize the Qwidget: top 33, right 9, left 9, down 39
	// origin is the coordinate where you press the mouse
	// terminal is the coordinate where you release the mouse

	// If user press mouse or release mouse on the border, origin and terminal coordinate will automatically place in the area of image slot
	QSize size = this->size();
	if(origin.rx() <= 9) {
		origin.rx() = 9;
	}
	if(origin.ry() <= 33) {
		origin.ry() = 33;
	}
	if(origin.rx() >= size.rwidth() - 9) {
		origin.rx() = size.rwidth() - 9;
	}

	if(origin.ry() >= size.rheight() - 39) {
		origin.ry() = size.rheight() - 39;
	}
	if(terminal.rx() <= 9) {
		terminal.rx() = 9;
	}
	if(terminal.ry() <= 33) {
		terminal.ry() = 33;
	}
	if(terminal.rx() >= size.rwidth() - 9) {
		terminal.rx() = size.rwidth() - 9;
	}
	if(terminal.ry() >= size.rheight() - 39) {
		terminal.ry() = size.rheight() - 39;
	}


	int widgetHeight = size.rheight();
	int widgetWidth = size.rwidth();

	//int initialImageHeight = target->image->height();
	//int initialImageWidth = target->image->width();
	int initialImageHeight = initial->image->height();
	int initialImageWidth = initial->image->width();

	int scaledImageHeight = 0;
	int scaledImageWidth = 0;

	if((double) initialImageHeight / (double) initialImageWidth > (double) 307 / (double) 619) { // the image slot on Qwidget is 619(width)  * 307(height)
	// so if the ratio of the initial image is greater than 307/ 619, which mean
		scaledImageHeight = widgetHeight - 72;
		scaledImageWidth = scaledImageHeight * initialImageWidth / initialImageHeight;

		if(origin.rx() < (size.rwidth() / 2 - scaledImageWidth / 2)) {
			origin.rx() = (size.rwidth() / 2 - scaledImageWidth / 2);
		}

		if(terminal.rx() < (size.rwidth() / 2 - scaledImageWidth / 2)) {
			terminal.rx() = (size.rwidth() / 2 - scaledImageWidth / 2);
		}

		if(terminal.rx() > (size.rwidth() / 2 + scaledImageWidth / 2)) {
			terminal.rx() = (size.rwidth() / 2 + scaledImageWidth / 2);
		}

		if(origin.rx() > (size.rwidth() / 2 + scaledImageWidth / 2)) {
			origin.rx() = (size.rwidth() / 2 + scaledImageWidth / 2);
		}
	}
	else { // height y
		scaledImageWidth = widgetWidth - 18;
		scaledImageHeight = scaledImageWidth * initialImageHeight / initialImageWidth;

		if(origin.ry() < ((size.rheight() - 72) / 2 + 33) - scaledImageHeight / 2) {
			origin.ry() = ((size.rheight() - 72) / 2 + 33) - scaledImageHeight / 2;
		}

		if(terminal.ry() < ((size.rheight() - 72) / 2 + 33) - scaledImageHeight / 2) {
			terminal.ry() = ((size.rheight() - 72) / 2 + 33) - scaledImageHeight / 2;
		}

		if(terminal.ry() > ((size.rheight() - 72) / 2 + 33) + scaledImageHeight / 2) {
			terminal.ry() = ((size.rheight() - 72) / 2 + 33) + scaledImageHeight / 2;
		}

		if(origin.ry() > ((size.rheight() - 72) / 2 + 33) + scaledImageHeight / 2) {
			origin.ry() = ((size.rheight() - 72) / 2 + 33) + scaledImageHeight / 2;
		}
	}


}

// User presses reset button to set page to original configuration
void TargetSelector::reset() {
	// delete qimage object and pathway for target. set pointer to null
	try {
		target->reset();
	}
	catch(...) { // catch everything
		QMessageBox messageBox;
		messageBox.warning(0, "Error", "Reset failed");
	}
	updateImage();
}

