#include <QtWidgets/QApplication>
#include <QStackedWidget>

#include "backgroundchooser.h"
#include "imagewizard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageWizard w;
    BackgroundChooser bc;

    QStackedWidget* frames = w.findChild<QStackedWidget*>("frames");
    frames->addWidget(&bc);

    w.show();
    return a.exec();
}
