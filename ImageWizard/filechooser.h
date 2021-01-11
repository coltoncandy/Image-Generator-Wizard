#pragma once

#include <QtWidgets/QWidget>
#include <qfiledialog>
#include "ui_filechooser.h"

class FileChooser : public QWidget
{
    Q_OBJECT

public:
    FileChooser(const QString& title, QWidget* parent = Q_NULLPTR);
    ~FileChooser();
    QImage* getImage(); 

public slots:
    void chooseFile();
    void setFilePath(QString url);

private:
    Ui::FileChooserClass ui;
    QFileDialog chooser;
    QLineEdit* chosenFileName;
    QImage* selectedImage;
};
