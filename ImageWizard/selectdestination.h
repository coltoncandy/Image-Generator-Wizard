 #pragma once

#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <QFileDialog>
#include "ui_selectdestination.h"

#include "imageinfo.h"
#include "wizardpage.h"

class SelectDestination : public WizardPage {
	Q_OBJECT

public:
	SelectDestination(const QString& title, QString* const path, QWidget* parent = Q_NULLPTR);
	~SelectDestination();

	void reset();
	bool isReady();

public slots:
	void setDirectory();

private:
	Ui::SelectDestinationClass ui;
	QLineEdit* chosenDestination;
	QString* destinationPath;
};