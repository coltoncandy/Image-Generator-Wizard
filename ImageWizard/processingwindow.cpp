#include "processingwindow.h"
#include "imagewizard.h"

#include <QLabel>
#include <QImage>
#include <QlineEdit>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>

ProcessingWindow::ProcessingWindow(const QString& title, QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QLabel* titleLabel = findChild<QLabel*>("title"); // show title on Qwidget
	titleLabel->setText(title);
}

ProcessingWindow::~ProcessingWindow()
{
}
