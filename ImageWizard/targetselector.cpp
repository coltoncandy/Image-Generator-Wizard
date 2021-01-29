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

	terminal.setX(-1);
	terminal.setY(-1);

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
	scaleImage(imgLabel->size());
}

void TargetSelector::updateImage() {
	imgLabel->setPixmap(QPixmap::fromImage(*(this->initial->image)));
	scaleImage(imgLabel->size());
}


// Initial selection box click 
void TargetSelector::mousePressEvent(QMouseEvent* e) {
	// create a try and catch. if the target->loaded true then dont let the press occur
	try {
		if(target->loaded)
			throw "Image already cropped. Press retry button to try again";
	}
	catch(const char * warn) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", warn);
	}
	origin = e->pos(); //origin of first click is set to position 
	
	QSize size = this->size();
	if(origin.rx() <= ui.imgLabel->x()) {
		origin.rx() = ui.imgLabel->x();
	}
	if(origin.ry() <= ui.imgLabel->y()) {
		origin.ry() = ui.imgLabel->y();
	}
	if(origin.rx() >= ui.imgLabel->x() + ui.imgLabel->width()) {
		origin.rx() = ui.imgLabel->x() + ui.imgLabel->width();
	}

	if(origin.ry() >= ui.imgLabel->y() + ui.imgLabel->height()) {
		origin.ry() = ui.imgLabel->y() + ui.imgLabel->height();
	}
	
	int widgetHeight = size.rheight();
	int widgetWidth = size.rwidth();

	int initialImageHeight = initial->image->height();
	int initialImageWidth = initial->image->width();

	int scaledImageHeight = 0;
	int scaledImageWidth = 0;

	if((double) initialImageHeight / (double) initialImageWidth > (double) ui.imgLabel->height() / (double) ui.imgLabel->width()) { // the image slot on Qwidget is 619(width)  * 307(height)
	// so if the ratio of the initial image is greater than 307/ 619, which mean
		scaledImageHeight = ui.imgLabel->height();
		scaledImageWidth = scaledImageHeight * initialImageWidth / initialImageHeight;

		if(origin.rx() < (size.rwidth() / 2 - scaledImageWidth / 2)) {
			origin.rx() = (size.rwidth() / 2 - scaledImageWidth / 2);
		}

		if(origin.rx() > (size.rwidth() / 2 + scaledImageWidth / 2)) {
			origin.rx() = (size.rwidth() / 2 + scaledImageWidth / 2);
		}
	}
	else if((double) initialImageHeight / (double) initialImageWidth < (double) ui.imgLabel->height() / (double) ui.imgLabel->width()) { // height y
		scaledImageWidth = ui.imgLabel->width();
		scaledImageHeight = scaledImageWidth * initialImageHeight / initialImageWidth;

		if(origin.ry() < (ui.imgLabel->height() / 2 + ui.imgLabel->y()) - scaledImageHeight / 2) {
			origin.ry() = (ui.imgLabel->height() / 2 + ui.imgLabel->y()) - scaledImageHeight / 2;
		}

		if(origin.ry() > (ui.imgLabel->height() / 2 + ui.imgLabel->y()) + scaledImageHeight / 2) {
			origin.ry() = (ui.imgLabel->height() / 2 + ui.imgLabel->y()) + scaledImageHeight / 2;
		}
	}


	if(!rubberBand)
		rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
	rubberBand->setGeometry(QRect(origin, QSize()));
	rubberBand->show();
}

// Allows selection box to follow users movement
void TargetSelector::mouseMoveEvent(QMouseEvent* e) {
	terminal = e->pos();

	QSize size = this->size();
	if(terminal.rx() <= ui.imgLabel->x()) {
		terminal.rx() = ui.imgLabel->x();
	}
	if(terminal.ry() <= ui.imgLabel->y()) {
		terminal.ry() = ui.imgLabel->y();
	}
	if(terminal.rx() >= ui.imgLabel->x() + ui.imgLabel->width()) {
		terminal.rx() = ui.imgLabel->x() + ui.imgLabel->width();
	}
	if(terminal.ry() >= ui.imgLabel->y() + ui.imgLabel->height()) {
		terminal.ry() = ui.imgLabel->y() + ui.imgLabel->height();
	}

	int widgetHeight = size.rheight();
	int widgetWidth = size.rwidth();

	int initialImageHeight = initial->image->height();
	int initialImageWidth = initial->image->width();

	int scaledImageHeight = 0;
	int scaledImageWidth = 0;

	if((double) initialImageHeight / (double) initialImageWidth > (double) ui.imgLabel->height() / (double) ui.imgLabel->width()) { // the image slot on Qwidget is 619(width)  * 307(height)
	// so if the ratio of the initial image is greater than 307/ 619, which mean
		scaledImageHeight = ui.imgLabel->height();
		scaledImageWidth = scaledImageHeight * initialImageWidth / initialImageHeight;

		if(terminal.rx() < (size.rwidth() / 2 - scaledImageWidth / 2)) {
			terminal.rx() = (size.rwidth() / 2 - scaledImageWidth / 2);
		}

		if(terminal.rx() > (size.rwidth() / 2 + scaledImageWidth / 2)) {
			terminal.rx() = (size.rwidth() / 2 + scaledImageWidth / 2);
		}
	}
	else if((double) initialImageHeight / (double) initialImageWidth < (double) ui.imgLabel->height() / (double) ui.imgLabel->width()) { // height y
		scaledImageWidth = ui.imgLabel->width();
		scaledImageHeight = scaledImageWidth * initialImageHeight / initialImageWidth;

		if(terminal.ry() < (ui.imgLabel->height() / 2 + ui.imgLabel->y()) - scaledImageHeight / 2) {
			terminal.ry() = (ui.imgLabel->height() / 2 + ui.imgLabel->y()) - scaledImageHeight / 2;
		}

		if(terminal.ry() > (ui.imgLabel->height() / 2 + ui.imgLabel->y()) + scaledImageHeight / 2) {
			terminal.ry() = (ui.imgLabel->height() / 2 + ui.imgLabel->y()) + scaledImageHeight / 2;
		}
	}

	rubberBand->setGeometry(QRect(origin, terminal).normalized());
}

// User releases mouse. Image is then cropped then shown
void TargetSelector::mouseReleaseEvent(QMouseEvent* event) {
	rubberBand->hide();

	QSize size = this->size();
	
	int x1 = origin.rx();
	int x2 = terminal.rx();
	int y1 = origin.ry();
	int y2 = terminal.ry();

	try {
		// user just clicks the image
		if(terminal.rx() == -1 || terminal.ry() == -1) {
			throw 0;
		}
		// handles user selecting a rectangle not on the image
		if(  (abs(origin.rx() - terminal.rx()) == 0) ||  (abs(origin.ry() - terminal.ry()) == 0) ) {
			throw "Must select a rectangle on the image";
		}
		// user chooses too small of square
		if(abs(origin.rx() - terminal.rx()) <= 10 || abs(origin.ry() - terminal.ry()) <= 10) {
			throw "Select a larger area on the image ";
		}

	}
	catch(int n) {
		reset();
		return;
	}
	catch(const char* warn) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", warn);
		reset();
		return;
	}
	

	int widgetHeight = size.rheight();
	int widgetWidth = size.rwidth();

	int initialImageHeight = initial->image->height();
	int initialImageWidth = initial->image->width();

	int scaledImageHeight = 0;
	int scaledImageWidth = 0;

	int xMin = 0;
	int yMin = 0;
	int xMax = 0;
	int yMax = 0;
	
	if((double) initialImageHeight / (double) initialImageWidth > (double) ui.imgLabel->height() / (double) ui.imgLabel->width()) {
		scaledImageHeight = ui.imgLabel->height();
		scaledImageWidth = scaledImageHeight * initialImageWidth / initialImageHeight;

		if(origin.ry() < terminal.ry()) {
			yMin = origin.ry() - ui.imgLabel->y();
			yMax = terminal.ry() - ui.imgLabel->y();
		}
		else {
			yMin = terminal.ry() - ui.imgLabel->y();
			yMax = origin.ry() - ui.imgLabel->y();
		}

		if(origin.rx() < terminal.rx()) {
			xMin = origin.rx() - ui.imgLabel->x() - (widgetWidth / 2 - scaledImageWidth / 2 - ui.imgLabel->x());
			xMax = terminal.rx() - ui.imgLabel->x() - (widgetWidth / 2 - scaledImageWidth / 2 - ui.imgLabel->x());
		}
		else {
			xMin = terminal.rx() - ui.imgLabel->x() - (widgetWidth / 2 - scaledImageWidth / 2 - ui.imgLabel->x());
			xMax = origin.rx() - ui.imgLabel->x() - (widgetWidth / 2 - scaledImageWidth / 2 - ui.imgLabel->x());
		}
	}
	else if((double) initialImageHeight / (double) initialImageWidth < (double) ui.imgLabel->height() / (double) ui.imgLabel->width()) {
		scaledImageWidth = ui.imgLabel->width();
		scaledImageHeight = scaledImageWidth * initialImageHeight / initialImageWidth;

		if(origin.rx() < terminal.rx()) {
			xMin = origin.rx() - ui.imgLabel->x();
			xMax = terminal.rx() - ui.imgLabel->x();
		}
		else {
			xMin = terminal.rx() - ui.imgLabel->x();
			xMax = origin.rx() - ui.imgLabel->x();
		}

		if(origin.ry() < terminal.ry()) {
			yMin = origin.ry() - ui.imgLabel->y() - ((ui.imgLabel->height() / 2) - (scaledImageHeight / 2));
			yMax = terminal.ry() - ui.imgLabel->y() - ((ui.imgLabel->height() / 2) - (scaledImageHeight / 2));
		}
		else {
			yMin = terminal.ry() - ui.imgLabel->y() - ((ui.imgLabel->height() / 2) - (scaledImageHeight / 2));
			yMax = origin.ry() - ui.imgLabel->y() - ((ui.imgLabel->height() / 2) - (scaledImageHeight / 2));
		}
	}
	else {
		scaledImageWidth = ui.imgLabel->width();
		scaledImageHeight = ui.imgLabel->height();

		if(origin.rx() < terminal.rx()) {
			xMin = origin.rx() - ui.imgLabel->x();
			xMax = terminal.rx() - ui.imgLabel->x();
		}
		else {
			xMin = terminal.rx() - ui.imgLabel->x();
			xMax = origin.rx() - ui.imgLabel->x();
		}

		if(origin.ry() < terminal.ry()) {
			yMin = origin.ry() - ui.imgLabel->y();
			yMax = terminal.ry() - ui.imgLabel->y();
		}
		else {
			yMin = terminal.ry() - ui.imgLabel->y();
			yMax = origin.ry() - ui.imgLabel->y();
		}
	}

	int newX = initialImageWidth * xMin / scaledImageWidth;
	int newY = initialImageHeight * yMin / scaledImageHeight;
	int newHeight = initialImageHeight * yMax / scaledImageHeight - newY;
	int newWidth = initialImageWidth * xMax / scaledImageWidth - newX;
	
	try {
		*(target->image) = initial->image->copy(newX, newY, newWidth, newHeight); // copying qimage to qimage
	}
	catch(...) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", "Something went wrong with copy");
		reset();
		return;
	}
	try {
		QPixmap imageCrop = QPixmap::fromImage(*(target->image));
		QSize siz = imgLabel->size();
		imgLabel->setPixmap(imageCrop.scaled(siz.width(), siz.height(), Qt::KeepAspectRatio)); //show "image" in the second QLabel
	}
	catch(...) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", "Something went wrong with display");
		reset();
		return;
	}
	*(target->path) = QDir::currentPath() + "/cropped.png"; //set path to current directory
	// pop up window error if save isn't successful. resets widget
	try {
		if(!target->image->save(*(target->path), "PNG", 100))
			throw "Save failed, select target again";
	}
	catch(const char* warn) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", warn);
		reset();
		return;
	}
	
	target->loaded = true;
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
	terminal.setX(-1);
	terminal.setY(-1);
	updateImage();
}

