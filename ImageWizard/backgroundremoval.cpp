#include "backgroundremoval.h"
#include "imagewizard.h"

BackgroundRemoval::BackgroundRemoval(const QString& title, QWidget* parent)
	: WizardPage(parent) {
	ui.setupUi(this);
	QLabel* titleLabel = findChild<QLabel*>("title");
	titleLabel->setText(title);
	QLabel* instructions = findChild<QLabel*>("instructions");
	instructions->setText("<b>TJs List</b><br><ul><li>1. Do something with N</li><li>2. Do something with ESC</li></ul>");
}

BackgroundRemoval::~BackgroundRemoval() {
}
