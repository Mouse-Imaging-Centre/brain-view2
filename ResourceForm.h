#ifndef RESOURCEFORM_H_
#define RESOURCEFORM_H_

#define HAVE_MINC2 1

#include "ui_resourceForm.h"
#include "ui_tagProps.h"
#include "ui_labelProps.h"
#include "TreeModel.h"
#include "BrainQuarter.h"
#include <QFile>
#include <QTextStream>

#include <QtGui>

class GeometryScene;

class ResourceForm : public QWidget {
	Q_OBJECT
public:
	ResourceForm(QWidget *parent, BrainQuarter *viewer);
	virtual ~ResourceForm();
	Ui::resourceForm ui;
	Ui::tagProps uitag;
	Ui::labelProps uilabel;
	bool insertGeometry(QFile &filename);
	bool insertVertstats(QFile &filename);
	bool insertTagfile(QFile &filename);
	bool insertLabelLUfile(QFile &filename);
	float * addTagpoint();
	double returnTagsize();
	void setLastGeometry( GeometryScene *geom) { lastGeometry = geom; };
	void somethingChanged();
	void updatebckgrndColour(QColor &colour);
	
	void setVerboseStatus(bool stat);
	bool getVerboseStatus();
	void emitVerboseSignal();
	bool ifVerbose;

	//for labale.config file
	QVector <int> labelnums;
	QVector <QString> labelnames;
	QVector <float> labelred;
	QVector <float> labelgreen;
	QVector <float> labelblue;

	signals:
		void verboseSignal();
		
public slots:
	void setPropertyForm(const QModelIndex & index);
	void selectedItem(const QModelIndex & index);
	void geometrySelected(int childNum);
	void enableCreateTag();
	void enableSaveTag();
	void disableAddTag();
// 	void enableSaveLabel();
	//void updateTagSize(double newsize);//{/* do nothing */};
private:
	QWidget *propWidget;
	QVBoxLayout *propLayout;
	QVBoxLayout *tagLayout;
	QWidget *tagWidget;
	TreeItem *currentPropItem;
	TreeItem *parent;
	TreeModel *model;
	BrainQuarter *viewer;
	bool haveProps;
	bool haveGeometry;
	GeometryScene *lastGeometry;
	void sampleTreeSetup(BrainQuarter *viewer);
	
	QWidget *labelWidget;
	QVBoxLayout *labelLayout;
	QVector <int> h5childCount;


};

#endif /*RESOURCEFORM_H_*/
