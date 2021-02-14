#pragma once

#include <QWidget>
#include "ui_previewimage.h"
#include "wizardpage.h"


class PreviewImage : public WizardPage
{
	Q_OBJECT

public:
	PreviewImage::PreviewImage(const QString& title, QString* directoryPath, QWidget* parent = Q_NULLPTR);
	~PreviewImage();

private:
	Ui::PreviewImageClass ui;
};
