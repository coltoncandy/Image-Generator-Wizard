#include "backgroundremoval.h"
#include "imagewizard.h"

BackgroundRemoval::BackgroundRemoval(const QString& title, QWidget* parent)
	: WizardPage(parent) {
	ui.setupUi(this);
	QLabel* titleLabel = findChild<QLabel*>("title");
	titleLabel->setText(title);
}

BackgroundRemoval::~BackgroundRemoval() {
}
