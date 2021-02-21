#include "foregroundchooser.h"

ForegroundChooser::ForegroundChooser(const QString& title, ImageInfo* image, const QString& originalPath, const QString& croppedPath, QWidget* parent): FileChooser(title, image, originalPath, parent)
{
	checkbox->show();
	QObject::connect(checkbox, &QCheckBox::stateChanged, this, &ForegroundChooser::changeRandomDir);
	originalDirectory = originalPath;
	croppedDirectory = croppedPath;
}

void ForegroundChooser::changeRandomDir(int state) {
	if(state == 2)
		defaultDirectory = QFileInfo(croppedDirectory);
	else
		defaultDirectory = QFileInfo(originalDirectory);
}

bool ForegroundChooser::skipCrop() {
	return checkbox->isChecked();
}
