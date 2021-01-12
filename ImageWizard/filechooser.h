﻿#pragma once

#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <QFileDialog>

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
    void setFilePath(QString url); //槽

    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

private:
    Ui::FileChooserClass ui;
    QFileDialog chooser; //选择文件的对话框
    QLineEdit* chosenFileName; //记录选择文件的名字
    
    void setupView();
    void loadImage(QString& path);

    //(Topher and TJ): We're not sure if we need a "backgroundchooser" class or just a "chooser" class, since it seems like it would be the same code for background vs initial image.
    QImage* selectedImage;
};
