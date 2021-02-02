#pragma once

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include "ui_welcomepage.h"
#include "wizardpage.h"

class WelcomePage : public WizardPage {
	Q_OBJECT

public:
	WelcomePage(const QString& title, QWidget* parent = Q_NULLPTR);
	~WelcomePage();

	bool isReady();

private:
	Ui::WelcomePage ui;
};
