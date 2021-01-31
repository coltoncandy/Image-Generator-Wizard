#include "selectdestination.h"
#include "imagewizard.h"

#include <QLabel>
#include <QImage>
#include <QlineEdit>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>

SelectDestination::SelectDestination(const QString& title, QString* destination, QWidget* parent) {
	QObject::connect(&chooser, &QFileDialog::directoryEntered, this, &SelectDestination::setDirectory);

	ui.setupUi(this);

	chooser.setFileMode(QFileDialog::FileMode::Directory);

	QLabel* titleLabel = findChild<QLabel*>("title"); // show title on Qwidget
	titleLabel->setText(title);

	destinationPath = destination;

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

}

void SelectDestination::chooseDirectory() {
	
	chooser.show();
}

void SelectDestination::setDirectory(QString path) {
	chosenDestination->setText(path);

	*(destinationPath) = path;
	ready = true;

	ImageWizard* wizard = dynamic_cast<ImageWizard*>(parent()->parent());
	wizard->enableNext();
}


