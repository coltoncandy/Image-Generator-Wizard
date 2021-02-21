#include "targetselector.h"
#include <QImage>
#include <QMouseEvent>
#include <QMainWindow>
#include <QDir>
#include <QMessageBox>

#include "imagewizard.h"

TargetSelector::TargetSelector(const QString& title, ImageInfo* initial, ImageInfo* target, QWidget* parent) : WizardPage(parent), rubberBand(0) {
	ui.setupUi(this);

	resetButton = findChild<QPushButton*>("reset");
	resetButton->setIcon(QIcon("  "));
	resetButton->setStyleSheet("border-left: 10px transparent; border-right: 10px transparent;""border-top: 3px transparent; border-bottom: 3px transparent;"); // remove edges of button
	resetButton->setIconSize(QSize(100, 50));
	QObject::connect(resetButton, &QPushButton::released, this, &TargetSelector::reset);
	resetButton->setCursor(QCursor(Qt::PointingHandCursor));
	// Set reset button icon and hover icon
	QString reset = QDir::homePath() + "/source/repos/image-generator/icons/reset.png";
	QString styleSheet = "QPushButton#reset{ image: url(%1); background-repeat: no-repeat; } QPushButton:hover#reset{ image: url(%2); background-repeat: no-repeat; }";
	QString resetHover = QDir::homePath() + "/source/repos/image-generator/icons/resetHover.png";
	setStyleSheet(styleSheet.arg(reset).arg(resetHover));

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

// Display image properly in widget according to widget size
void TargetSelector::scaleImage(const QSize& size) {
	QPixmap p;
	// display target image properly 
	if(target->loaded) {
		p = QPixmap::fromImage(*(target->image));
		imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
	}
	// display initial image properly
	else {
		p = QPixmap::fromImage(*(initial->image));
		imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
	}
}

// scale image properly after window is resized
void TargetSelector::resizeEvent(QResizeEvent* e) {
	QWidget::resizeEvent(e);
	scaleImage(imgLabel->size());
}

// update how image is displayed
void TargetSelector::updateImage() {
	imgLabel->setPixmap(QPixmap::fromImage(*(this->initial->image)));
	scaleImage(imgLabel->size());
}

bool TargetSelector::isReady() {
	return target->loaded;
}

void TargetSelector::resetCoordiate(QPoint& point) {

	// origin is the coordinate of where user press the mouse
	// terminal is the coordiante of where user release the mouse
	// ui.imgLabel is the slot that place the image
	// once we have origin and terminal, we can get the cropped rectangle's coordiate

	// if origin/terminal out of ui.imgLabel, it will automatically round to the area of ui.imgLabel
	// ui.imgLabel coordinate range: [ui.imgLabel->x() , ui.imgLabel->x() + ui.imgLabel->width()]
	// ui.imgLabel coordinate range: [ui.imgLabel->x() , ui.imgLabel->x() + ui.imgLabel->width()]
	QSize size = this->size();

	if(point.rx() <= ui.imgLabel->x()) {
		point.rx() = ui.imgLabel->x();
	}
	else if(point.rx() >= ui.imgLabel->x() + ui.imgLabel->width()) {
		point.rx() = ui.imgLabel->x() + ui.imgLabel->width();
	}
	if(point.ry() <= ui.imgLabel->y()) {
		point.ry() = ui.imgLabel->y();
	}
	else if(point.ry() >= ui.imgLabel->y() + ui.imgLabel->height()) {
		point.ry() = ui.imgLabel->y() + ui.imgLabel->height();
	}

	// get size of widget in real-time
	int widgetHeight = size.rheight();
	int widgetWidth = size.rwidth();

	int initialImageHeight = initial->image->height();
	int initialImageWidth = initial->image->width();

	int scaledImageHeight = 0;
	int scaledImageWidth = 0;

	// the ratio of ui.imgLabel is  ui.imgLabel->height() / ui.imgLabel->width()
	// and initial image will keep aspect ratio and place in ui.imgLabel with 
	// if the ratio(height/width) of initial image is larger than ui.imgLabel ratio, remove the blank area on two side in the image 
	if((double) initialImageHeight / (double) initialImageWidth > (double) ui.imgLabel->height() / (double) ui.imgLabel->width()) {
		scaledImageHeight = ui.imgLabel->height();
		scaledImageWidth = scaledImageHeight * initialImageWidth / initialImageHeight;

		if(point.rx() < (size.rwidth() / 2 - scaledImageWidth / 2)) {
			point.rx() = (size.rwidth() / 2 - scaledImageWidth / 2);
		}
		else if(point.rx() > (size.rwidth() / 2 + scaledImageWidth / 2)) {
			point.rx() = (size.rwidth() / 2 + scaledImageWidth / 2);
		}
	}
	// if the ratio(height/width) of initial image is larger than ui.imgLabel ratio, remove the blank area on top and buttom side in the image 
	else if((double) initialImageHeight / (double) initialImageWidth < (double) ui.imgLabel->height() / (double) ui.imgLabel->width()) { // height y
		scaledImageWidth = ui.imgLabel->width();
		scaledImageHeight = scaledImageWidth * initialImageHeight / initialImageWidth;

		if(point.ry() < (ui.imgLabel->height() / 2 + ui.imgLabel->y()) - scaledImageHeight / 2) {
			point.ry() = (ui.imgLabel->height() / 2 + ui.imgLabel->y()) - scaledImageHeight / 2;
		}
		else if(point.ry() > (ui.imgLabel->height() / 2 + ui.imgLabel->y()) + scaledImageHeight / 2) {
			point.ry() = (ui.imgLabel->height() / 2 + ui.imgLabel->y()) + scaledImageHeight / 2;
		}
	}
}


// Initial selection box click 
void TargetSelector::mousePressEvent(QMouseEvent* e) {
	// create a try and catch. if the target->loaded true then dont let the press occur
	try {
		if(target->loaded)
			throw "Image already cropped. Press retry button to try again";
	}
	catch(const char* warn) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", warn);
	}
	origin = e->pos(); //origin of first click is set to position 

	resetCoordiate(origin);
	QSize size = this->size();
	// select rubberband geometry and set up corner of drag box
	if(!rubberBand)
		rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
	rubberBand->setGeometry(QRect(origin, QSize()));
	rubberBand->show();
}

// Allows selection box to follow users movement
void TargetSelector::mouseMoveEvent(QMouseEvent* e) {
	terminal = e->pos();
	resetCoordiate(terminal);
	rubberBand->setGeometry(QRect(origin, terminal).normalized());
}

// User releases mouse. Image is then cropped then shown
void TargetSelector::mouseReleaseEvent(QMouseEvent* event) {
	rubberBand->hide();

	QSize size = this->size();

	try {
		// user just clicks the image
		if(terminal.rx() == -1 || terminal.ry() == -1) {
			throw 0;
		}
		// handles user selecting a rectangle not on the image
		if((abs(origin.rx() - terminal.rx()) == 0) || (abs(origin.ry() - terminal.ry()) == 0)) {
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

	// calculate the absolute coordinate for the points for origin and terminal for scaled image, which the new coordinate is not for Qwidget, it's for scaled image
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

	// Use X,Y,X,Y,Height,Width for cropped rectangle on scaled image and initial image size to calculate the X,Y,Height,Width for cropped rectangle on initial image
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

	getWizard()->enableNext();
}

// User presses reset button to set page to original configuration
void TargetSelector::reset() {
	WizardPage::reset();

	getWizard()->disableNext();
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

void TargetSelector::pageSwitched() {
	if(target->loaded) {
		reset();
	}
	*(target->image) = *(initial->image);
	updateImage();
}