#include "previewimage.h"
#include "wizardpage.h"
#include <QLabel>




PreviewImage::PreviewImage(const QString& title, QString* directoryPath, QWidget *parent)
	: WizardPage(parent)
{
	ui.setupUi(this);
}

PreviewImage::~PreviewImage()
{
}
