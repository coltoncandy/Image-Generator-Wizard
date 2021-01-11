#include "imagewizard.h"

ImageWizard::ImageWizard(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    frames = findChild<QStackedWidget*>("frames");
}
//Adds pages to the private 'frames' variable, representative of windows in the UI 
void ImageWizard::addFrame(FileChooser& frame) {
    frames->addWidget(&frame); 
}
//Next page in UI
void ImageWizard::goNext() {
    int cur = frames->currentIndex();
    if (cur < frames->count()) {
        frames->setCurrentIndex(++cur);
    }
}
//Previous page in UI 
void ImageWizard::goPrev() {
    int cur = frames->currentIndex();
    if (cur > 0) {
        frames->setCurrentIndex(--cur);
    }
}
