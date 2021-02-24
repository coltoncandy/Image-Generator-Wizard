#pragma once

#include "ui_batchchoice.h"
#include "wizardpage.h"

class BatchChoice : public WizardPage {
	Q_OBJECT

public:
	BatchChoice(QWidget* parent = nullptr);
	~BatchChoice();

	void pageSwitched();

public slots:
	void chooseSingle();
	void chooseBatch();

private:
	Ui::BatchChoiceClass ui;
};