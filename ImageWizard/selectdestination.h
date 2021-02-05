#pragma once

#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <QFileDialog>
#include "ui_selectdestination.h"

class SelectDestination : public QWidget
{
	Q_OBJECT

public:
	SelectDestination(const QString& title, QString* destination ,QWidget* parent = Q_NULLPTR);
	~SelectDestination();

	bool isReady();
	
public slots:
	void chooseDirectory();
	void reset();
	void setDirectory(QString url);
	QString* getDestination();
	

private:
	Ui::SelectDestinationClass ui;
	QFileDialog chooser;
	QLineEdit* chosenDestination;
	QString* destinationPath;
	QPushButton* resetButton;
	bool ready;

};


