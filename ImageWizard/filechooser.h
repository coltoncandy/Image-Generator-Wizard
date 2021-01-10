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

public slots:
    void chooseFile();
    void setFilePath(QString url);

private:
    Ui::FileChooserClass ui;
    QFileDialog chooser;
    QLineEdit* chosenFileName;

    //(Topher and TJ): We're not sure if we need a "backgroundchooser" class or just a "chooser" class, since it seems like it would be the same code for background vs initial image.
    QImage* selectedImage;
};
