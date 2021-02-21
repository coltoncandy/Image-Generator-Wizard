
#include <QMessageBox>
#include <QFileDialog>

#include "batchoptions.h"
#include "imagewizard.h"

BatchOptions::BatchOptions(QString* const path, QWidget* parent) : WizardPage(parent) {
	ui.setupUi(this);

	chosenDestination = findChild<QLineEdit*>("chosenDestination");
	destinationPath = path;

	numUnique = findChild<QSpinBox*>("numUnique");
	batchSize = findChild<QLineEdit*>("batchSize");
	batchSize->setValidator(new QIntValidator(0, 10000, this));
}

BatchOptions::~BatchOptions() {

}

bool BatchOptions::isReady() {
	return batchSize->isEnabled() && !batchSize->text().isEmpty();
}

void BatchOptions::setDirectory() {
	if(getWizard()->isNextEnabled())
		getWizard()->disableNext();
	try {
		*destinationPath = QFileDialog::getExistingDirectory(this,
							tr("Choose directory"),
							"",
							QFileDialog::DontResolveSymlinks);
		chosenDestination->setText(*destinationPath);
	}
	catch(...) {
		QMessageBox messageBox;
		messageBox.warning(0, "Error", "Could not load pathway");
	}

	numUnique->setEnabled(true);
	batchSize->setEnabled(true);
	batchSize->setText("1");
}

void BatchOptions::batchSizeChanged(QString text) {
	if(!text.isEmpty())
		getWizard()->enableNext();
	else
		getWizard()->disableNext();
}