#pragma once

#include <QLineEdit>
#include <QSpinBox>

#include "ui_batchoptions.h"
#include "wizardpage.h"

class BatchOptions : public WizardPage {
	Q_OBJECT

public:
	BatchOptions(QString* const path, QWidget* parent = nullptr);
	~BatchOptions();

	bool isReady();

public slots:
	void setDirectory();
	void batchSizeChanged(QString text);

private:
	Ui::BatchOptionsClass ui;
	QLineEdit* chosenDestination;
	QString* destinationPath;
	QSpinBox* numUnique;
	QLineEdit* batchSize;
};