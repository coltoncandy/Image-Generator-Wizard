#include <QtWidgets/QApplication>
#include <QStackedWidget>

#include "filechooser.h"
#include "imagewizard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageWizard wizard;
    FileChooser backgroundChooser("Select an image for the background");

    QStackedWidget* frames = wizard.findChild<QStackedWidget*>("frames");
    frames->addWidget(&backgroundChooser);
    frames->addWidget(new QWidget);

    wizard.show();
    return a.exec();
}
