#pragma once

#include <QWidget>
#include <QLabel>

#include "ui_backgroundremoval.h"
#include "wizardpage.h"

class BackgroundRemoval : public WizardPage {
	Q_OBJECT

public:
	BackgroundRemoval(const QString& title, QWidget* parent = Q_NULLPTR);
	~BackgroundRemoval();

private:
	Ui::BackgroundRemovalClass ui;
};
