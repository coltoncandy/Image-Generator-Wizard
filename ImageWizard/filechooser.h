#pragma once

#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <QFileDialog>

#include "ui_filechooser.h"

class FileChooser : public QWidget {
	Q_OBJECT

public:
	FileChooser(const QString& title, QWidget* parent = Q_NULLPTR);
	~FileChooser();
	QImage* getImage();

	void resizeEvent(QResizeEvent* e);

public slots:
	void chooseFile();
	void setFilePath(QString url);

	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);

private:
	Ui::FileChooserClass ui;
	QFileDialog chooser;
	QLineEdit* chosenFileName;
	QLabel* imgLabel;
	bool loaded;

	void setupView();
	void loadImage(QString& path);
	void scaleImage(const QSize& size);

	QImage* selectedImage;
};
