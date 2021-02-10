#pragma once

#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <QFileDialog>
#include "ui_selectdestination.h"

#include "imageinfo.h"
#include "wizardpage.h"

class SelectDestination : public WizardPage
{
	Q_OBJECT

public:
	SelectDestination(const QString& title, QString* destination ,QWidget* parent = Q_NULLPTR);
	~SelectDestination();

	bool isReady();
	
public slots:
	void chooseDirectory();
	void reset();
	void setDirectory(QString url);
	QString* getDestination();
	

private:
	Ui::SelectDestinationClass ui;
	QFileDialog chooser;
	QLineEdit* chosenDestination;
	QString* destinationPath;
	QPushButton* resetButton;
	bool ready;

};


