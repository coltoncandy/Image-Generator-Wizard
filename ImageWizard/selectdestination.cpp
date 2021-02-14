#include "selectdestination.h"
#include "imagewizard.h"

#include <QLabel>
#include <QImage>
#include <QlineEdit>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>


SelectDestination::SelectDestination(const QString& title, QString* destination, QWidget* parent) : WizardPage(parent) {
	QObject::connect(&chooser, &QFileDialog::directoryEntered, this, &SelectDestination::setDirectory);
	ui.setupUi(this);
	chooser.setFileMode(QFileDialog::FileMode::Directory);

	resetButton = findChild<QPushButton*>("reset");
	resetButton->setIcon(QIcon("  "));
	resetButton->setStyleSheet("border-left: 10px transparent; border-right: 10px transparent;""border-top: 3px transparent; border-bottom: 3px transparent;"); // remove edges of button
	resetButton->setIconSize(QSize(100, 50));
	QObject::connect(resetButton, &QPushButton::released, this, &SelectDestination::reset);
	resetButton->setCursor(QCursor(Qt::PointingHandCursor));
	// Set reset button icon and hover icon
	QString reset = QDir::homePath() + "/source/repos/image-generator/icons/reset.png";
	QString styleSheet = "QPushButton#reset{ image: url(%1); background-repeat: no-repeat; } QPushButton:hover#reset{ image: url(%2); background-repeat: no-repeat; }";
	QString resetHover = QDir::homePath() + "/source/repos/image-generator/icons/resetHover.png";
	setStyleSheet(styleSheet.arg(reset).arg(resetHover));

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
	*(destinationPath) = NULL;
	ImageWizard* wizard = dynamic_cast<ImageWizard*>(parent()->parent());
	wizard->disableNext();
	QLabel* titleLabel = findChild<QLabel*>("title"); // show title on Qwidget
	chosenDestination = findChild<QLineEdit*>("chosenDestination");
	chosenDestination->setText(" ");
}

void SelectDestination::chooseDirectory() {
	chooser.show();
}

QString* SelectDestination::getDestination() {
	return destinationPath;
}

void SelectDestination::setDirectory(QString path) {
	chosenDestination->setText(path);

	*(destinationPath) = path;
	ready = true;
	ImageWizard* wizard = dynamic_cast<ImageWizard*>(parent()->parent());
	wizard->enableNext();
}


