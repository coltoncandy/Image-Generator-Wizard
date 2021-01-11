#include <QtWidgets/QApplication>
#include <QStackedWidget>

#include "filechooser.h"
#include "imagewizard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageWizard wizard;
    FileChooser targetChooser("Select an image containing the target");
    FileChooser backgroundChooser("Select an image for the background");

    wizard.addFrame(targetChooser); 
    wizard.addFrame(backgroundChooser); 

    wizard.show();
    return a.exec();
}
