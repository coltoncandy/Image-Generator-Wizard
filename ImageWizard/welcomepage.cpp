#include "welcomepage.h"

WelcomePage::WelcomePage(const QString& title, QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//QPixmap pix("C:/Users/Tula/Desktop/Quadcopter.png");
	QPixmap pix("C:/Users/Tula/Pictures/Quadcopter4.png");
	ui.Drone_logo->setPixmap(pix);
	QLabel* titleLabel = findChild<QLabel*>("title");
	//QTextEdit* welcomeMessage = findChild<QTextEdit*>("welcomeMessage");
	titleLabel->setText(title);
	//welcomeMessage->setHtml("<p>This program will help you generate unique images containing a selected target object. These images are free to use under the GNU General Public License v3. Press the \"->\" button to begin generating your images.<\\p>");
}

WelcomePage::~WelcomePage()
{
}
