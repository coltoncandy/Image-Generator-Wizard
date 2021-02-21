#pragma once

#include <QWidget>
#include "ui_previewimage.h"
#include "wizardpage.h"
#include "imageinfo.h"
#include <opencv2/core.hpp>

class PreviewImage : public WizardPage {
	Q_OBJECT

public:
	PreviewImage::PreviewImage(const QString& title, cv::Mat& processedImage, QWidget* parent = Q_NULLPTR);
	~PreviewImage();
	void pageSwitched();

public slots:
	void PreviewImage::resizeEvent(QResizeEvent*);

private:
	Ui::PreviewImageClass ui;
	QLabel* imgLabel;
	//ImageInfo* processedImage;
	cv::Mat& imageMat;
	QImage image;
	void loadImage();
	void scaleImage(const QSize&);
	void reset();
	QImage result;
};