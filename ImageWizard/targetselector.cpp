#include "targetselector.h"
//#include <QLabel>
#include <QImage>
// Added
#include <QMouseEvent>
#include <QAction>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>


TargetSelector::TargetSelector(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

}

TargetSelector::~TargetSelector()
{
}

void TargetSelector::setImage(QImage* image) {
	initialImage = image;
	QLabel* imgLabel = findChild<QLabel*>("imgLabel");  
	imgLabel->setPixmap(QPixmap::fromImage(*(this->initialImage)));
}

//open up png file as png in C++
//create mouse events to get coordinates... and give in ratio of total size to png editor in C++


void TargetSelector::paintEvent(QPaintEvent* e) {
	QWidget::paintEvent(e);
	QPainter painter(this);
	painter.setPen(QPen(QBrush(QColor(0, 0, 0, 180)), 1, Qt::DashLine));
	painter.setBrush(QBrush(QColor(255, 255, 255, 120)));
	painter.drawRect(selectionRect);
}

void TargetSelector::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::RightButton) {
		if (selectionRect.contains(e->pos()))
			contextMenu.exec(this->mapToGlobal(e->pos()));
	}
	else {
		selectionStarted = true;
		selectionRect.setTopLeft(e->pos());
		selectionRect.setBottomRight(e->pos());
	}
}

void TargetSelector::mouseMoveEvent(QMouseEvent* e)
{
	if (selectionStarted) {
		selectionRect.setBottomRight(e->pos());
		repaint();
	}
}

void TargetSelector::mouseReleaseEvent(QMouseEvent* event)
{
	selectionStarted = false;
}

/*
void TargetSelector::paintEvent(QPaintEvent* e)
{
	//QWidget::paintEvent(e);
	QPainter painter(this);
	painter.setPen(QPen(QBrush(QColor(0, 0, 0, 180)), 1, Qt::DashLine));
	painter.setBrush(QBrush(QColor(255, 255, 255, 120)));
	painter.drawRect(selectionRect);
}



void TargetSelector::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::RightButton)
	{
		if (selectionRect.contains(e->pos()))
			contextMenu.exec(this->mapToGlobal(e->pos()));
	}
	else
	{
		selectionStarted = true;
		selectionRect.setTopLeft(e->pos());
		selectionRect.setBottomRight(e->pos());
	}
}

void TargetSelector::mouseMoveEvent(QMouseEvent*e)
{
	if (selectionStarted)
	{
		selectionRect.setBottomRight(e->pos());
		repaint();
	}
}

void TargetSelector::mouseReleaseEvent(QMouseEvent *e)
{
	selectionStarted = false;
}


void TargetSelector::saveSlot()
{
	QString fileName = QFileDialog::getSaveFileName(this, QObject::tr("Save File"), "/home", QObject::tr("Images(*.jpg)"));
	this->pixmap()->copy(selectionRect).save(fileName);
}
*/