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
    QString chooseFile();

private:
    Ui::BackgroundChooserClass ui;
    QFileDialog chooser;
};
