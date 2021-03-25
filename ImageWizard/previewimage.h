#pragma once

#include <QWidget>
#include <QPushButton>
#include "ui_previewimage.h"
#include "wizardpage.h"
#include "imageinfo.h"
#include <opencv2/core.hpp>
#include <vector>
#include <string>
#include "../AlgoManager/filemanager.h"
#include "../AlgoManager/algomanager.h"

class PreviewImage : public WizardPage {
	Q_OBJECT

public:
	PreviewImage(const QString& title, QWidget* parent = Q_NULLPTR);
	void pageSwitched(int imageNum, const std::string& targetPath, const std::string& backgroundPath, const std::string& destination, bool batchFlag);

public slots:
	void resizeEvent(QResizeEvent*);
	void saveImage();
	void nextImage();
	void processAgain();

private:
	Ui::PreviewImageClass ui;
	QLabel* imgLabel;
	std::vector<cv::Mat> imageMats;
	QLabel* titleLabel;
	QPushButton* nextImageButton;
	QPushButton* saveButton;
	QPushButton* processButton;
	QString processingTitle;
	QString batchTitle;
	QString singleImageTitle;
	QString failedProcessingTitle;
	std::string targetPath;
	std::string backgroundPath;
	std::string destination;
	QImage image;
	bool batchFlag;
	int imageIndex;
	int imageNum;
	int batchSize;

	void batchProcess();
	void process();
	void loadImage();
	void scaleImage(const QSize&);
	void reset();
};