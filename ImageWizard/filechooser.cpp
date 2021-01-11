#include "filechooser.h"

#include <QLabel>
#include <QImage>
#include <QlineEdit>

FileChooser::FileChooser(const QString& title, QWidget* parent)
    : QWidget(parent)
{
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
}

FileChooser::~FileChooser() {
    delete selectedImage;
}

void FileChooser::chooseFile() {
    chooser.show();
}

void FileChooser::setFilePath(QString path) {
    chosenFileName->setText(path);

    if (this->selectedImage)
        delete this->selectedImage;

    this->selectedImage = new QImage(path);
    // Easiest way to display an image is to set the pixmap of a label
    QLabel* imgLabel = findChild<QLabel*>("imgLabel");
    imgLabel->setPixmap(QPixmap::fromImage(*(this->selectedImage)));
    //(Topher and TJ): Saving the image to a private variable will let us access it later (but we don't want to do it yet in case it causes memory leaks, so leaving it commented for now)
    //backgroundImage = img
}