#pragma once

#include <QtWidgets/QWidget>
#include <qfiledialog>
#include "ui_backgroundchooser.h"

class BackgroundChooser : public QWidget
{
    Q_OBJECT

public:
    BackgroundChooser(QWidget* parent = Q_NULLPTR);

public slots:
    void chooseFile();
    void setFilePath(QString url);

private:
    Ui::BackgroundChooserClass ui;
    QFileDialog chooser;
    QLineEdit* chosenFileName;

    //(Topher and TJ): We're not sure if we need a "backgroundchooser" class or just a "chooser" class, since it seems like it would be the same code for background vs initial image.
    QImage* backgroundImage;
    QImage* initialImage;
    QImage* objectCropped;
};
