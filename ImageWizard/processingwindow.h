#pragma once

#include <QtWidgets/QWidget>
#include <QMainWindow>
#include "ui_processingwindow.h"
#include "imageinfo.h"
#include "wizardpage.h"
#include <opencv2/core.hpp>

class ProcessingWindow : public WizardPage {
	Q_OBJECT

public:
	ProcessingWindow(const QString& title, cv::Mat& image, const ImageInfo* const initialImage, const ImageInfo* const targetImage, const ImageInfo* const backgroundImage, QWidget* parent = Q_NULLPTR);
	~ProcessingWindow();
	void pageSwitched();

private:
	Ui::ProcessingWindow ui;
	cv::Mat processingImage;
	const ImageInfo* const initial;
	const ImageInfo* const target;
	const ImageInfo* const background;
};
