#include "welcomepage.h"

WelcomePage::WelcomePage(const QString& title, QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QPixmap pix("C:/Users/Tula/Desktop/Q.png");
	ui.Drone_logo->setPixmap(pix);
	QLabel* titleLabel = findChild<QLabel*>("title");
	titleLabel->setText(title);
}

WelcomePage::~WelcomePage()
{
}
