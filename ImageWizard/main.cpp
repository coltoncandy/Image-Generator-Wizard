#include "imagewizard.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageWizard w;
    w.show();
    return a.exec();
}
