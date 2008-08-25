#ifndef RESOURCEFORM_H_
#define RESOURCEFORM_H_

#include "ui_resourceForm.h"
#include "TreeModel.h"

class ResourceForm : public QWidget {
	Q_OBJECT
public:
	ResourceForm(QWidget *parent);
	virtual ~ResourceForm();
private:
	Ui::resourceForm ui;
	void sampleTreeSetup();
};

#endif /*RESOURCEFORM_H_*/
