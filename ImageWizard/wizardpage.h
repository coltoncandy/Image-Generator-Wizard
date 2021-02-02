#pragma once

#include <QWidget>

class ImageWizard;

class WizardPage : public QWidget {
	Q_OBJECT

public:
	WizardPage(QWidget* parent = nullptr);
	virtual ~WizardPage();

	virtual bool isReady();
	virtual void pageSwitched();

public slots:
	virtual void reset();

protected:
	ImageWizard* getWizard();
};