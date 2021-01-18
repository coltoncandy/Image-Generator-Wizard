#pragma once

#include <QImage>

struct ImageInfo {
	QImage* image;
	QString* path;

	ImageInfo() {
		image = new QImage;
		path = new QString;
	}

	~ImageInfo() {
		delete image;
		delete path;
	}
};