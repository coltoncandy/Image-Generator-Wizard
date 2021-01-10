#include "backgroundchooser.h"

BackgroundChooser::BackgroundChooser(QWidget* parent)
    : QWidget(parent)
{
    QObject::connect(&chooser, &QFileDialog::fileSelected, this, &BackgroundChooser::setFileUrl);

    ui.setupUi(this);
    chooser.setFileMode(QFileDialog::FileMode::ExistingFile);

    chosenFileName = findChild<QLineEdit*>("chosenFileName");
}

void BackgroundChooser::chooseFile() {
    chooser.show();
}


void BackgroundChooser::setFileUrl(QString url) {
    chosenFileName->setText(url);
}