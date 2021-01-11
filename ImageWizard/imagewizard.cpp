#include "imagewizard.h"

ImageWizard::ImageWizard(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    frames = findChild<QStackedWidget*>("frames");
}

void ImageWizard::goNext() {
    int cur = frames->currentIndex();
    if (cur < frames->count()) {
        frames->setCurrentIndex(++cur);
    }
}

void ImageWizard::goPrev() {
    int cur = frames->currentIndex();
    if (cur > 0) {
        frames->setCurrentIndex(--cur);
    }
}
