#include <QtWidgets/QApplication>
#include <QStackedWidget>
#include <QMessageBox>

#include "filechooser.h"
#include "imagewizard.h"

int main(int argc, char* argv[]) {
	
	srand(time(NULL)); 
	QApplication a(argc, argv);
	
	// set up stylesheet
	QFile styleFile(QDir::currentPath() + "/Resource Files/stylesheet.qss");
	try {
		if(!styleFile.open(QFile::ReadOnly))
			throw (QDir::currentPath() + "/Resource Files/stylesheet.qss");
	}
	catch(std::string pathway) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", "Stylesheet cannot open at: " + QString::fromStdString(pathway));
		exit(0);
	}
	QString style(styleFile.readAll());
	styleFile.close();
	a.setStyleSheet(style);
	
	ImageWizard wizard;

	wizard.show();

	return a.exec();
}
