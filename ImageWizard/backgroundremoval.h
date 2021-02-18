#pragma once

#include <QWidget>
#include <QLabel>

#include "ui_backgroundremoval.h"
#include "wizardpage.h"
#include "imageinfo.h"


class BackgroundRemoval : public WizardPage {
	Q_OBJECT

public:
	BackgroundRemoval(const QString& title, ImageInfo* target, QWidget* parent = Q_NULLPTR);
	~BackgroundRemoval();
	void pageSwitched();
	void disableButton();
	void enableButton();

public slots:
	void editButton();

private:
	
	Ui::BackgroundRemovalClass ui;
	ImageInfo* targetImage;
	QLabel* imgLabel;
	QPushButton* cropButton;

};
