#pragma once

#include <QtWidgets/QWidget>
#include <QMainWindow>
#include "ui_processingwindow.h"
#include "imageinfo.h"
#include "wizardpage.h"

class ProcessingWindow : public WizardPage
{
	Q_OBJECT

public:
	ProcessingWindow(const QString& title, QWidget* parent = Q_NULLPTR);
	~ProcessingWindow();

private:
	Ui::ProcessingWindow ui;
};
