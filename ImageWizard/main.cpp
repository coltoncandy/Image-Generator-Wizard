#include <QtWidgets/QApplication>

#include "backgroundchooser.h"
#include "imagewizard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageWizard w;
    BackgroundChooser bc(&w);

    w.show();
    return a.exec();
}
