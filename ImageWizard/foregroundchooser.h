#pragma once

#include "FileChooser.h"

class ForegroundChooser : public FileChooser
{
	Q_OBJECT

public:
	ForegroundChooser(const QString& title, ImageInfo* image, const QString& originalPath, const QString& croppedPath, QWidget* parent = Q_NULLPTR);
	bool skipCrop();

public slots:
	void changeRandomDir(int state);

private:
	QString originalDirectory;
	QString croppedDirectory;
};
