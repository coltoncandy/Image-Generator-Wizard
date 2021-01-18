#pragma once

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include "ui_welcomepage.h"

class WelcomePage : public QWidget
{
	Q_OBJECT

public:
	WelcomePage(const QString& title = nullptr, QWidget* parent = Q_NULLPTR);
	~WelcomePage();

private:
	Ui::WelcomePage ui;
};
