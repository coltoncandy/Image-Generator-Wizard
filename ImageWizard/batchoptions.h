#pragma once

#include "ui_batchoptions.h"
#include "wizardpage.h"

class BatchOptions : public WizardPage {
	Q_OBJECT

public:
	BatchOptions(QString* const path, QWidget* parent = nullptr);
	~BatchOptions();

public slots:
	void setDirectory();

private:
	Ui::BatchOptionsClass ui;
	QLineEdit* chosenDestination;
	QString* destinationPath;
};