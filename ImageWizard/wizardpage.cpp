
#include "imagewizard.h"
#include "wizardpage.h"

WizardPage::WizardPage(QWidget* parent) : QWidget(parent) {
}

WizardPage::~WizardPage() {

}

// This is used by ImageWizard to determine whether the next button should be
// enable when switching BACK to the page, since state is kept
// THIS IS NECESSARY, if there is nothing it depends on just return true.
// Otherwise, when going backwards this page's next button will be disable.
// HOWEVER, dynamic pages, like target selector and filechooser, still need to update
// ImageWizard's enableNext when certain actions are performed
bool WizardPage::isReady() {
	return false;
}

// This is used for page-specific actions that need to be performed upon being switched
// FORWARD to, e.g. TargetSelector copying image data
void WizardPage::pageSwitched() {

}

// This is used by the back and reset buttons for some pages. 
void WizardPage::reset() {

}

// This is just a shortcut to access wizard functions, like enableNext
ImageWizard* WizardPage::getWizard() {
	return dynamic_cast<ImageWizard*>(parent()->parent());
}