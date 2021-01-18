﻿#include "filechooser.h"

#include <QLabel>
#include <QImage>
#include <QlineEdit>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>

FileChooser::FileChooser(const QString& title, QWidget* parent)
	: QWidget(parent) {
	QObject::connect(&chooser, &QFileDialog::fileSelected, this, &FileChooser::setFilePath);

	ui.setupUi(this);

	// this makes it so we can only choose a single file that exists; no directories or multiple files
	chooser.setFileMode(QFileDialog::FileMode::ExistingFile);

	// only allow us to choose files with the following extensions
	chooser.setNameFilter("Images (*.jpg *.jpeg *.png)");

	chosenFileName = findChild<QLineEdit*>("chosenFileName");
	QLabel* titleLabel = findChild<QLabel*>("title");
	titleLabel->setText(title);

	selectedImage = nullptr;

	setupView();
}

FileChooser::~FileChooser() {
	if(this->selectedImage)
		delete selectedImage;
}

QImage* FileChooser::getImage() {
	return this->selectedImage;
}

void FileChooser::chooseFile() {
	chooser.show();
}

void FileChooser::setFilePath(QString path) {
	chosenFileName->setText(path);

	loadImage(path);
}

void FileChooser::dragEnterEvent(QDragEnterEvent* event) {
	event->acceptProposedAction();
}

void FileChooser::dropEvent(QDropEvent* event) {
	QString url = event->mimeData()->urls().first().toLocalFile();
	chosenFileName = findChild<QLineEdit*>("chosenFileName");
	chosenFileName->setText(url);
	if(url.isEmpty()) {
		return;
	}
	;
	loadImage(url);
}

void FileChooser::setupView() {
	this->setAcceptDrops(true);
}


void FileChooser::loadImage(QString& path) {
	if(this->selectedImage)
		delete this->selectedImage;

	this->selectedImage = new QImage(path);
	// Easiest way to display an image is to set the pixmap of a label
	QLabel* imgLabel = findChild<QLabel*>("imgLabel");
	imgLabel->setPixmap(QPixmap::fromImage(*(this->selectedImage)));
}