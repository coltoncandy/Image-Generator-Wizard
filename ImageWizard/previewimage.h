#pragma once

#include <QWidget>
#include "ui_previewimage.h"
#include "wizardpage.h"
#include "imageinfo.h"
#include <opencv2/core.hpp>

class PreviewImage : public WizardPage
{
	Q_OBJECT

public:
	PreviewImage::PreviewImage(const QString& title, cv::Mat img, QWidget* parent = Q_NULLPTR);
	~PreviewImage();
	void updateImage(cv::Mat img);

public slots:
	void PreviewImage::resizeEvent(QResizeEvent*);

private:
	Ui::PreviewImageClass ui;
	QLabel* imgLabel;
	ImageInfo* processedImage;
	void loadImage(cv::Mat img);
	void scaleImage(const QSize&);
	void reset();
};