#include "ResourceForm.h"
#include <QtGui>
#include "TreeItem.h"


ResourceForm::ResourceForm(QWidget *parent, BrainQuarter *viewer) : QWidget(parent){
	ui.setupUi(this);
	sampleTreeSetup(viewer);
}

ResourceForm::~ResourceForm() {
}

void ResourceForm::sampleTreeSetup(BrainQuarter *viewer) {
	QStringList headers;
	headers << tr("Title") << tr("Description");

	QFile file("/usr/lib/qt4/examples/itemviews/editabletreemodel/default.txt");
	file.open(QIODevice::ReadOnly);
	TreeModel *model = new TreeModel(headers, file.readAll());
	file.close();

	TreeItem *parent = model->getRootItem();
	parent->setViewer(viewer);
	//parent->insertCone();
	QFile f1("/micehome/jlerch/CLASP-119/test-out/00101/surfaces/mni_icbm_00101_gray_surface_left_81920.obj");
	QFile f2("/micehome/jlerch/CLASP-119/test-out/00101/surfaces/mni_icbm_00101_white_surface_right_81920.obj");
	parent->insertGeometry(f1);
	parent->insertGeometry(f2);
	//parent->insertChildren(parent->childCount(), 1, parent->columnCount() );
	//parent->child(parent->childCount() - 1)->setData(0, "testing");
	
	ui.treeView->setModel(model);
}