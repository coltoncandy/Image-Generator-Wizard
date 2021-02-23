
#include <cstdlib>
#include <string>

#include <QMessageBox>
#include <QFileDialog>

#include "batchoptions.h"
#include "imagewizard.h"
#include "filechooser.h"

BatchOptions::BatchOptions(BatchInfo* batchInfo, QWidget* parent) : WizardPage(parent), batchInfo(batchInfo) {
	ui.setupUi(this);

	chosenDestination = findChild<QLineEdit*>("chosenDestination");

	numUnique = findChild<QSpinBox*>("numUnique");
	batchSize = findChild<QLineEdit*>("batchSize");
	batchSize->setValidator(new QIntValidator(1, 9999, this));

	numUnique->hide();
}

BatchOptions::~BatchOptions() {

}

bool BatchOptions::isReady() {
	return batchSize->isEnabled() && !batchSize->text().isEmpty();
}

void BatchOptions::reset() {
	chosenDestination->setText("");
	numUnique->setEnabled(false);
	batchSize->setEnabled(false);
	batchSize->setText("");
	batchInfo->reset();
}

void BatchOptions::setDirectory() {
	if(getWizard()->isNextEnabled())
		getWizard()->disableNext();
	try {
		batchInfo->directory = (QFileDialog::getExistingDirectory(this,
							tr("Choose directory"),
							"",
							QFileDialog::DontResolveSymlinks)).toStdString();
		chosenDestination->setText(batchInfo->directory.c_str());
	}
	catch(...) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", "Could not load pathway");
	}

	numUnique->setEnabled(true);
	batchSize->setEnabled(true);
	batchSize->setText("1");

	//std::string directory = QFileInfo(*destinationPath).absoluteFilePath().toStdString();
	//int numImages = imageCount(directory, batchInfo->files);
	//numUnique->setMaximum(numImages);
	//numUnique->setValue(numImages);
}

void BatchOptions::batchSizeChanged(QString text) {
	if(!text.isEmpty()) {
		batchInfo->batchSize = atoi(batchSize->text().toStdString().c_str());
		if(batchInfo->batchSize > 0)
			getWizard()->enableNext();
		else
			getWizard()->disableNext();
	}
	else
		getWizard()->disableNext();
}

void BatchOptions::numUniqueChanged(int value) {
	// don't worry about this for now
}