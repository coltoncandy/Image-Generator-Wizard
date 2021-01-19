#include "filechooser.h"

#include <QLabel>
#include <QImage>
#include <QlineEdit>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QMessageBox>

FileChooser::FileChooser(const QString& title, ImageInfo* image, QWidget* parent) : QWidget(parent) {
	QObject::connect(&chooser, &QFileDialog::fileSelected, this, &FileChooser::setFilePath);

	ui.setupUi(this);

	// this makes it so we can only choose a single file that exists; no directories or multiple files
	chooser.setFileMode(QFileDialog::FileMode::ExistingFile);

	// only allow us to choose files with the following extensions

	acceptedFileTypes = {
		"png",
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

	selectedImage = image;

	setupView();
}

FileChooser::~FileChooser() {
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
	selectedImage->image->load(path);
	*(selectedImage->path) = path;
	selectedImage->loaded = true;

	scaleImage(imgLabel->size());
}

void FileChooser::scaleImage(const QSize& size) {
	if(!selectedImage->loaded)
		return;

	QPixmap p = QPixmap::fromImage(*(selectedImage->image));
	imgLabel->setPixmap(p.scaled(size.width(), size.height(), Qt::KeepAspectRatio));
}

void FileChooser::resizeEvent(QResizeEvent* e) {
	QWidget::resizeEvent(e);
	scaleImage(imgLabel->size());
}