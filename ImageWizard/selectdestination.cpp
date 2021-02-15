#include "selectdestination.h"
#include "imagewizard.h"

#include <QLabel>
#include <QImage>
#include <QlineEdit>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>

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

}

bool SelectDestination::isReady() {
	return !destinationPath->isEmpty();
}

void SelectDestination::setDirectory() {

	*destinationPath = QFileDialog::getExistingDirectory(this,
						tr("Choose directory"),
						"",
						QFileDialog::DontResolveSymlinks);
	chosenDestination->setText(*destinationPath);

	if(isReady())
		getWizard()->enableNext();
}