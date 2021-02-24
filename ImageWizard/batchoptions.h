#pragma once

#include <vector>
#include <string>

#include <QLineEdit>
#include <QSpinBox>

#include "ui_batchoptions.h"
#include "wizardpage.h"
#include "../AlgoManager/filemanager.h"

struct BatchInfo {
	std::vector<std::string> files;
	std::string directory;
	int numUnique;
	int batchSize;
	bool doBatch;

	BatchInfo() : numUnique(0), batchSize(0), doBatch(false) {}

	void reset() {
		files.clear();
		directory.clear();
		numUnique = 0;
		batchSize = 0;
		doBatch = false;
	}
};

class BatchOptions : public WizardPage {
	Q_OBJECT

public:
	BatchOptions(BatchInfo* batchInfo, QWidget* parent = nullptr);
	~BatchOptions();

	bool isReady();
	void reset();

public slots:
	void setDirectory();
	void batchSizeChanged(QString text);
	void numUniqueChanged(int value);

private:
	Ui::BatchOptionsClass ui;
	QLineEdit* chosenDestination;
	QSpinBox* numUnique;
	QLineEdit* batchSize;
	BatchInfo* batchInfo;
};