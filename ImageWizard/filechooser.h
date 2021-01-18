#pragma once

#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <QFileDialog>

#include "ui_filechooser.h"
#include "imageinfo.h"

class FileChooser : public QWidget {
	Q_OBJECT

public:
	FileChooser(const QString& title, ImageInfo* image, QWidget* parent = Q_NULLPTR);
	~FileChooser();

public slots:
	void chooseFile();
	void setFilePath(QString url);

	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);

private:
	Ui::FileChooserClass ui;
	QFileDialog chooser;
	QLineEdit* chosenFileName;

	void setupView();
	void loadImage(QString& path);

	ImageInfo* selectedImage;
};
