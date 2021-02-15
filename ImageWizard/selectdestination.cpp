#include "selectdestination.h"
#include "imagewizard.h"

#include <QlineEdit>
#include <QMessageBox>

SelectDestination::SelectDestination(const QString& title, QString* const path, QWidget* parent) : WizardPage(parent) {

	ui.setupUi(this);

	QLabel* titleLabel = findChild<QLabel*>("title"); // show title on Qwidget
	titleLabel->setText(title);

	chosenDestination = findChild<QLineEdit*>("chosenDestination");

	destinationPath = path;
}

SelectDestination::~SelectDestination() {
}

void SelectDestination::reset() {
	destinationPath = new QString;
	chosenDestination->setText(*destinationPath);
}

bool SelectDestination::isReady() {
	return !destinationPath->isEmpty();
}

void SelectDestination::setDirectory() {
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