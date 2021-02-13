#include "processingwindow.h"
#include "imagewizard.h"

ProcessingWindow::ProcessingWindow(const QString& title, QWidget* parent)
	: WizardPage(parent)
{
	ui.setupUi(this);

	QLabel* titleLabel = findChild<QLabel*>("title"); // show title on Qwidget
	titleLabel->setText(title);
}

ProcessingWindow::~ProcessingWindow()
{
}
