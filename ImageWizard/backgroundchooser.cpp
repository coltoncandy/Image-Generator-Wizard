#include "backgroundchooser.h"

#include <QLabel>
#include <QImage>

BackgroundChooser::BackgroundChooser(QWidget* parent)
    : QWidget(parent)
{
    QObject::connect(&chooser, &QFileDialog::fileSelected, this, &BackgroundChooser::setFilePath);

    ui.setupUi(this);

    // this makes it so we can only choose a single file that exists; no directories or multiple files
    chooser.setFileMode(QFileDialog::FileMode::ExistingFile);

    chosenFileName = findChild<QLineEdit*>("chosenFileName");
}

void BackgroundChooser::chooseFile() {
    chooser.show();
}

void BackgroundChooser::setFilePath(QString path) {
    chosenFileName->setText(path);

    QImage* img = new QImage(path);
    // Easiest way to display an image is to set the pixmap of a label
    QLabel* lab = findChild<QLabel*>("label");
    lab->setPixmap(QPixmap::fromImage(*img));
    delete img;
}