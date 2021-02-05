#include <QtWidgets/QApplication>
#include <QStackedWidget>

#include "filechooser.h"
#include "imagewizard.h"

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	//QFile stream = QFile("C:/Users/source/repos/image-generator/ImageWizard/Resource Files/stylesheet.qss");
	//stream.open(QIODevice.ReadOnly);
	//QFile styleFile("C:/Users/source/repos/image-generator/ImageWizard/Resource Files/stylesheet.qss");
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
	/*

	QFile file(QDir::currentPath() + "/Resource Files/stylesheet.qss");
	//	QFile file("C:/Users/Tula/source/repos/image-generator/ImageWizard/Resource Files/stylesheet.qss");
	try {
		if(!file.open(QFile::ReadOnly))
			throw false;
	}
	catch(bool f) {
		exit(0);
	}
	QString style = QLatin1String(file.readAll());
	a.setStyleSheet(style);
	*/
	ImageWizard wizard;

	wizard.show();

	return a.exec();
}
