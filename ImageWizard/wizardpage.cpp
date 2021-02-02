
#include "imagewizard.h"
#include "wizardpage.h"

WizardPage::WizardPage(QWidget* parent) : QWidget(parent) {
}

WizardPage::~WizardPage() {

}

bool WizardPage::isReady() {
	return false;
}

void WizardPage::pageSwitched() {

}

void WizardPage::reset() {

}

ImageWizard* WizardPage::getWizard() {
	return dynamic_cast<ImageWizard*>(parent()->parent());
}