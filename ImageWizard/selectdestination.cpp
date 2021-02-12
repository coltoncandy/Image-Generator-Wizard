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

	chosenDestination = findChild<QLineEdit*>("chosenDestination");
	
}

SelectDestination::~SelectDestination() {
}

bool SelectDestination::isReady() {
	return ready;
}

void SelectDestination::reset() {
	ready = false;
	destinationPath = NULL;
	ImageWizard* wizard = dynamic_cast<ImageWizard*>(parent()->parent());
	wizard->disableNext();
	QLabel* titleLabel = findChild<QLabel*>("title"); // show title on Qwidget
	chosenDestination = findChild<QLineEdit*>("chosenDestination");
	chosenDestination->setText(" ");
}

QString SelectDestination::getDestination() {
	return destinationPath;
}

void SelectDestination::setDirectory() {

	destinationPath = QFileDialog::getExistingDirectory(this,
						tr("Choose directory"),
						"/home",
						QFileDialog::DontResolveSymlinks);
	chosenDestination->setText(destinationPath);

	ready = true;
	ImageWizard* wizard = dynamic_cast<ImageWizard*>(parent()->parent());
	wizard->enableNext();
}


