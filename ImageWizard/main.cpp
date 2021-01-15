#include <QtWidgets/QApplication>
#include <QStackedWidget>

#include "filechooser.h"
#include "imagewizard.h"
#include "../AlgoManager/AlgoManager.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageWizard wizard;
   
    wizard.show();

    return a.exec();
}
