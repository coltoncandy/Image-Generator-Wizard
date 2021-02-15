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
	SelectDestination(const QString& title, QWidget* parent = Q_NULLPTR);
	~SelectDestination();

	bool isReady();
	
public slots:
	void setDirectory();
	QString * getDestination();
	

private:
	Ui::SelectDestinationClass ui;
	QLineEdit* chosenDestination;
	QString destinationPath;
	QString enmptyPath;
	QPushButton* resetButton;
	QString* destination;
	bool ready;

};


