#pragma once

#include <QWidget>
#include "ui_previewimage.h"
#include "wizardpage.h"
#include "imageinfo.h"


class PreviewImage : public WizardPage
{
	Q_OBJECT

public:
	PreviewImage::PreviewImage(const QString& title, QWidget* parent = Q_NULLPTR);
	~PreviewImage();
	void updateImage(const QString*);

public slots:
	void PreviewImage::resizeEvent(QResizeEvent*);

private:
	Ui::PreviewImageClass ui;
	QLabel* imgLabel;
	ImageInfo* processedImage;
	void loadImage(const QString*);
	void scaleImage(const QSize&);
	void reset();
};