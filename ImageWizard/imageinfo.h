#pragma once

#include <QImage>

struct ImageInfo {
	QImage* image;
	QString* path;
	bool loaded;

	ImageInfo() {
		image = new QImage;
		path = new QString;
		loaded = false;
	}

	~ImageInfo() {
		delete image;
		delete path;
	}

	void reset() {
		delete image;
		delete path;
		image = new QImage;
		path = new QString;
		loaded = false;
	}
};