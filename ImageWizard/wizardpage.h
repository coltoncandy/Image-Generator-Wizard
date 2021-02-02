#pragma once

#include <QWidget>

class WizardPage : public QWidget {
	Q_OBJECT

public:
	WizardPage(QWidget* parent = nullptr) : QWidget(parent) {}
	virtual ~WizardPage() {}

	virtual bool isReady() {
		return false;
	}

	virtual void pageSwitched() {}

public slots:
	virtual void reset() {}

private:
};