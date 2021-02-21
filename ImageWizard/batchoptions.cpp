
#include <QMessageBox>
#include <QFileDialog>

#include "batchoptions.h"
#include "imagewizard.h"

BatchOptions::BatchOptions(QString* const path, QWidget* parent) : WizardPage(parent) {
	ui.setupUi(this);

	chosenDestination = findChild<QLineEdit*>("chosenDestination");
	destinationPath = path;
}

BatchOptions::~BatchOptions() {

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

	if(isReady())
		getWizard()->enableNext();
}