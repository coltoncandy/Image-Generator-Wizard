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

	/*resetButton = findChild<QPushButton*>("reset");
	resetButton->setIcon(QIcon("  "));
	resetButton->setStyleSheet("border-left: 10px transparent; border-right: 10px transparent;""border-top: 3px transparent; border-bottom: 3px transparent;"); // remove edges of button
	resetButton->setIconSize(QSize(100, 50));
	QObject::connect(resetButton, &QPushButton::released, this, &SelectDestination::reset);
	resetButton->setCursor(QCursor(Qt::PointingHandCursor));
	// Set reset button icon and hover icon
	QString reset = QDir::homePath() + "/source/repos/image-generator/icons/reset.png";
	QString styleSheet = "QPushButton#reset{ image: url(%1); background-repeat: no-repeat; } QPushButton:hover#reset{ image: url(%2); background-repeat: no-repeat; }";
	QString resetHover = QDir::homePath() + "/source/repos/image-generator/icons/resetHover.png";
	setStyleSheet(styleSheet.arg(reset).arg(resetHover));*/

	QLabel* titleLabel = findChild<QLabel*>("title"); // show title on Qwidget
	titleLabel->setText(title);

	destinationPath = NULL;
	enmptyPath = NULL;
	destination = NULL;
	ready = false;

	chosenDestination = findChild<QLineEdit*>("chosenDestination");

}

SelectDestination::~SelectDestination() {
}


bool SelectDestination::isReady() {
	return ready;
}

QString * SelectDestination::getDestination() {
	return destination;
}

void SelectDestination::setDirectory() {

	destinationPath = NULL;
	destinationPath = QFileDialog::getExistingDirectory(this,
						tr("Choose directory"),
						"",
						QFileDialog::DontResolveSymlinks);
	destination = &destinationPath;
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


