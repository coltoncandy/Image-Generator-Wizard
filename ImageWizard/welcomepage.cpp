#include "welcomepage.h"

WelcomePage::WelcomePage(const QString& title, QWidget* parent) : WizardPage(parent) {
	ui.setupUi(this);
	QLabel* titleLabel = findChild<QLabel*>("title");
	QTextEdit* welcomeMessage = findChild<QTextEdit*>("welcomeMessage");
	titleLabel->setText(title);
	welcomeMessage->setHtml("<p>This program will help you generate unique images containing a selected target object. These images are free to use under the GNU General Public License v3. Press the \"->\" button to begin generating your images.<\\p>");
}

WelcomePage::~WelcomePage() {
}

bool isReady() {
	return true;
}