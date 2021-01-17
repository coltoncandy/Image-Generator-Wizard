#include "filechooser.h"

#include <QLabel>
#include <QImage>
#include <QlineEdit>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QMessageBox>

FileChooser::FileChooser(const QString& title, QWidget* parent)
	: QWidget(parent) {
	QObject::connect(&chooser, &QFileDialog::fileSelected, this, &FileChooser::setFilePath);

	ui.setupUi(this);

	// this makes it so we can only choose a single file that exists; no directories or multiple files
	chooser.setFileMode(QFileDialog::FileMode::ExistingFile);

	// only allow us to choose files with the following extensions

	acceptedFileTypes = {
		"png",
		"jpg",
		"jpeg"
	};

	std::string fileFilter = "Images (";
	for(int i = 0; i < acceptedFileTypes.size(); i++) {
		fileFilter += "*." + acceptedFileTypes[i];

		if(i < acceptedFileTypes.size() - 1)
			fileFilter += " ";
	}
	fileFilter += ")";

	chooser.setNameFilter(fileFilter.c_str());

	chosenFileName = findChild<QLineEdit*>("chosenFileName");
	imgLabel = findChild<QLabel*>("imgLabel");
	QLabel* titleLabel = findChild<QLabel*>("title");
	titleLabel->setText(title);

	selectedImage = nullptr;
	loaded = false;

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
	if(url.isEmpty())
		return;
	
	bool validFileType = false;

	for(std::string fileType : acceptedFileTypes) {
		if(url.toUtf8().endsWith(fileType))
			validFileType = true;
	}

	if(validFileType) {
		chosenFileName->setText(url);
		loadImage(url);
	}
	else {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", "Invalid file type");
	}
}

void FileChooser::setupView() {
	this->setAcceptDrops(true);
}

void FileChooser::loadImage(QString& path) {
	if(this->selectedImage)
		delete this->selectedImage;

	this->selectedImage = new QImage(path);
	loaded = true;
	scaleImage(imgLabel->size());
}

void FileChooser::scaleImage(const QSize& size) {
	if(!loaded)
		return;

	QPixmap p = QPixmap::fromImage(*(this->selectedImage));
	imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
}

void FileChooser::resizeEvent(QResizeEvent* e) {
	QWidget::resizeEvent(e);
	scaleImage(imgLabel->size());
}