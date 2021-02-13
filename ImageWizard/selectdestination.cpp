#include "selectdestination.h"
#include "imagewizard.h"

#include <QLabel>
#include <QImage>
#include <QlineEdit>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>

SelectDestination::SelectDestination(const QString& title, QWidget* parent) : WizardPage(parent) {

	ui.setupUi(this);

	QLabel* titleLabel = findChild<QLabel*>("title"); // show title on Qwidget
	titleLabel->setText(title);

	destinationPath = NULL;
	enmptyPath = NULL;
	ready = false;

	chosenDestination = findChild<QLineEdit*>("chosenDestination");

}

SelectDestination::~SelectDestination() {
}


bool SelectDestination::isReady() {
	return ready;
}

QString SelectDestination::getDestination() {
	return destinationPath;
}

void SelectDestination::setDirectory() {

	destinationPath = NULL;
	destinationPath = QFileDialog::getExistingDirectory(this,
						tr("Choose directory"),
						"",
						QFileDialog::DontResolveSymlinks);
	chosenDestination->setText(destinationPath);

	if(destinationPath != enmptyPath) {
		ready = true;
		ImageWizard* wizard = dynamic_cast<ImageWizard*>(parent()->parent());
		wizard->enableNext();
	}
	else {
		ImageWizard* wizard = dynamic_cast<ImageWizard*>(parent()->parent());
		wizard->disableNext();
	}
}


