#ifndef RESOURCEFORM_H_
#define RESOURCEFORM_H_

#include "ui_resourceForm.h"
#include "TreeModel.h"
#include "BrainQuarter.h"

#include <QtGui>

class ResourceForm : public QWidget {
	Q_OBJECT
public:
	ResourceForm(QWidget *parent, BrainQuarter *viewer);
	virtual ~ResourceForm();
private:
	Ui::resourceForm ui;
	void sampleTreeSetup(BrainQuarter *viewer);
};

#endif /*RESOURCEFORM_H_*/
