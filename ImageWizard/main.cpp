#include <QtWidgets/QApplication>
#include <QStackedWidget>

#include "filechooser.h"
#include "imagewizard.h"

int main(int argc, char* argv[]) {
	
	srand(time(NULL)); 
	QApplication a(argc, argv);
	
	// set up stylesheet
	QFile styleFile(QDir::currentPath() + "/Resource Files/stylesheet.qss");
	try {
		if(!styleFile.open(QFile::ReadOnly))
			throw false;
	}
	catch(bool f) {
		exit(0);
	}
	QString style(styleFile.readAll());
	styleFile.close();
	a.setStyleSheet(style);
	
	ImageWizard wizard;

	wizard.show();

	return a.exec();
}
