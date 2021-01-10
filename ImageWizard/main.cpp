#include <QtWidgets/QApplication>
#include <QLayout>

#include "backgroundchooser.h"
#include "imagewizard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageWizard w;
    BackgroundChooser bc;
    w.layout()->addWidget(&bc);

    w.show();
    return a.exec();
}
