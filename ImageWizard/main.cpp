#include <QtWidgets/QApplication>
#include <QStackedWidget>

#include "filechooser.h"
#include "imagewizard.h"
#include "../AlgoManager/AlgoManager.cpp"

int main(int argc, char *argv[])
{
    //test function for static library... to be deleted
    fnAlgoManager();

    QApplication a(argc, argv);
    ImageWizard wizard;
   
    wizard.show();

    return a.exec();
}
