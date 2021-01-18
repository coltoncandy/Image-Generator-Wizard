#include "welcomepage.h"

WelcomePage::WelcomePage(const QString& title, QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QLabel* titleLabel = findChild<QLabel*>("title");
	QTextEdit* welcomeMessage = findChild<QTextEdit*>("welcomeMessage");
	titleLabel->setText(title);
	welcomeMessage->setHtml("<p>This program will help you create unique images containing a selected target object. These images are free to use under the GNU liscense. Press the \"->\" button to begin creating your images.<\\p>");
}

WelcomePage::~WelcomePage()
{
}
