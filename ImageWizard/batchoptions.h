#pragma once

#include "ui_batchoptions.h"
#include "wizardpage.h"

class BatchOptions : public WizardPage {
	Q_OBJECT

public:
	BatchOptions(QWidget* parent = nullptr);
	~BatchOptions();

private:
	Ui::BatchOptionsClass ui;
};