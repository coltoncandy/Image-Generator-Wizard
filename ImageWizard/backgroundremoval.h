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
	void reset();

public slots:
	void editButton();
	void resizeEvent(QResizeEvent* e);

private:
	
	Ui::BackgroundRemovalClass ui;
	ImageInfo* targetImage;
	QLabel* imgLabel;
	QPushButton* cropButton;
	bool enableDisplay;

	void scaleImage(const QSize& size);
	void grabCut();

};
