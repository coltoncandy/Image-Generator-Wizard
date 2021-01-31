#pragma once

#include <QWidget>
#include "ui_processingwindow.h"

class ProcessingWindow : public QWidget
{
	Q_OBJECT

public:
	ProcessingWindow(QWidget *parent = Q_NULLPTR);
	~ProcessingWindow();

private:
	Ui::ProcessingWindow ui;
};
