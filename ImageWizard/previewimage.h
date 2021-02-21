#pragma once

#include <QWidget>
#include "ui_previewimage.h"
#include "wizardpage.h"
#include "imageinfo.h"
#include <opencv2/core.hpp>

class PreviewImage : public WizardPage {
	Q_OBJECT

public:
	PreviewImage::PreviewImage(const QString& title, const cv::Mat& processedImage, const QString* const destinationPath, QWidget* parent = Q_NULLPTR);
	~PreviewImage();
	void pageSwitched();

public slots:
	void PreviewImage::resizeEvent(QResizeEvent*);

private:
	Ui::PreviewImageClass ui;
	QLabel* imgLabel;
	const cv::Mat& imageMat;
	const QString* const destination;
	QImage image;
	void loadImage();
	void scaleImage(const QSize&);
	void reset();
};