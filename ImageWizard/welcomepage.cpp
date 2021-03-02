#include "welcomepage.h"
#include "imagewizard.h"
#include <QDir>

WelcomePage::WelcomePage(const QString& title, QWidget* parent) : WizardPage(parent) {
	ui.setupUi(this);
	QPixmap pix(QDir::currentPath()+ "/icons/drone.png");
	ui.Drone_logo->setPixmap(pix);
	QLabel* titleLabel = findChild<QLabel*>("mainTitle");
	titleLabel->setText(title);
}

WelcomePage::~WelcomePage() {
}