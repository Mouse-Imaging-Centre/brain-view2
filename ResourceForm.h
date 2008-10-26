#ifndef RESOURCEFORM_H_
#define RESOURCEFORM_H_

//#include "GeometryScene.h"
#include "ui_resourceForm.h"
#include "TreeModel.h"
#include "BrainQuarter.h"
//#include "TreeItem.h"


#include <QtGui>

class GeometryScene;

class ResourceForm : public QWidget {
	Q_OBJECT
public:
	ResourceForm(QWidget *parent, BrainQuarter *viewer);
	virtual ~ResourceForm();
	Ui::resourceForm ui;
	bool insertGeometry(QFile &filename);
	bool insertVertstats(QFile &filename);
public slots:
	void setPropertyForm(const QModelIndex & index);
	void selectedItem(const QModelIndex & index);
private:
	QWidget *propWidget;
	QVBoxLayout *propLayout;
	TreeItem *currentPropItem;
	TreeItem *parent;
	TreeModel *model;
	BrainQuarter *viewer;
	bool haveProps;
	bool haveGeometry;
	GeometryScene *lastGeometry;
	void sampleTreeSetup(BrainQuarter *viewer);
	void somethingChanged();

};

#endif /*RESOURCEFORM_H_*/
