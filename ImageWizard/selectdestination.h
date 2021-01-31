#pragma once

#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <QFileDialog>
#include "ui_selectdestination.h"

class SelectDestination : public QWidget
{
	Q_OBJECT

public:
	SelectDestination(const QString& title, QString* destination ,QWidget* parent = Q_NULLPTR);
	~SelectDestination();

	bool isReady(); // implement this
	void reset(); // implement this

public slots:
	void chooseDirectory();
	void setDirectory(QString url);

private:
	Ui::SelectDestinationClass ui;
	QFileDialog chooser;
	QLineEdit* chosenDestination;
	QString* destinationPath;
	bool ready; //implement this

};


