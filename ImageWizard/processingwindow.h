#pragma once

#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <QFileDialog>
#include "ui_processingwindow.h"

class ProcessingWindow : public QWidget
{
	Q_OBJECT

public:
	ProcessingWindow(const QString& title, QWidget* parent = Q_NULLPTR);
	~ProcessingWindow();

private:
	Ui::ProcessingWindow ui;
};
