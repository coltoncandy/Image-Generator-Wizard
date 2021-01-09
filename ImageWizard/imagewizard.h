#pragma once

#include <QtWidgets/QWidget>
#include "ui_imagewizard.h"

class ImageWizard : public QWidget
{
    Q_OBJECT

public:
    ImageWizard(QWidget *parent = Q_NULLPTR);

private:
    Ui::ImageWizardClass ui;
};
