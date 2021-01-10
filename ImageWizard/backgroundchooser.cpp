#include "backgroundchooser.h"

BackgroundChooser::BackgroundChooser(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

QString BackgroundChooser::chooseFile() {

    chooser.show();

    return nullptr;
}