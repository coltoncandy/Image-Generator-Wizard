#include "filechooser.h"

#include <QLabel>
#include <QImage>
#include <QlineEdit>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>

FileChooser::FileChooser(const QString& title, QWidget* parent)
    : QWidget(parent)
{
    QObject::connect(&chooser, &QFileDialog::fileSelected, this, &FileChooser::setFilePath); // 发送者, 发送的信号， 接收者， 接受的槽

    ui.setupUi(this);

    // this makes it so we can only choose a single file that exists; no directories or multiple files
    chooser.setFileMode(QFileDialog::FileMode::ExistingFile);

    // only allow us to choose files with the following extensions
    chooser.setNameFilter("Images (*.jpg *.jpeg *.png)"); // 文件过滤器

    chosenFileName = findChild<QLineEdit*>("chosenFileName");  //设置路径的变量，链接到UI中相应的位置
    QLabel* titleLabel = findChild<QLabel*>("title");  //设置标题的变量，链接到UI中相应的位置
    titleLabel->setText(title); //放置标题

    selectedImage = nullptr;

    setupView();
}

FileChooser::~FileChooser() {
    delete selectedImage;
}

void FileChooser::chooseFile() { 
    chooser.show();
}

void FileChooser::setFilePath(QString path) {
    chosenFileName->setText(path);  //填充文件路径

    if (this->selectedImage) //如果有图片，删除图片
        delete this->selectedImage; 

    this->selectedImage = new QImage(path); 
    // Easiest way to display an image is to set the pixmap of a label
    QLabel* imgLabel = findChild<QLabel*>("imgLabel");  //设置图片的变量，链接到UI中相应的位置
    imgLabel->setPixmap(QPixmap::fromImage(*(this->selectedImage))); //放置图片
    //(Topher and TJ): Saving the image to a private variable will let us access it later (but we don't want to do it yet in case it causes memory leaks, so leaving it commented for now)
    //backgroundImage = img
}




void FileChooser::dragEnterEvent(QDragEnterEvent* event)
{
    event->acceptProposedAction();
}


void FileChooser::dropEvent(QDropEvent* event)
{
    QString url = event->mimeData()->urls().first().toLocalFile();   // 结果   "C:/User/test/Desktop/a.png"
    if (url.isEmpty()) {
        return;
    }

    //具体将拿到的数据进行处理
    //img.load(url);
    this->selectedImage = new QImage(url);
    QLabel* imgLabel = findChild<QLabel*>("imgLabel");  //设置图片的变量，链接到UI中相应的位置
    imgLabel->setPixmap(QPixmap::fromImage(*(this->selectedImage))); //放置图片
}

void FileChooser::setupView()
{
    this->setAcceptDrops(true);   
}
