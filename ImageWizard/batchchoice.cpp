
#include "imagewizard.h"
#include "batchchoice.h"

BatchChoice::BatchChoice(QWidget* parent) : WizardPage(parent) {
	ui.setupUi(this);
}

BatchChoice::~BatchChoice() {}


void BatchChoice::pageSwitched() {
	getWizard()->hideNext();
}

void BatchChoice::chooseSingle() {

}

void BatchChoice::chooseBatch() {

}