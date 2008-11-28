#include "ResourceForm.h"
#include <QtGui>
#include "GeometryScene.h"

#include <iostream>

ResourceForm::ResourceForm(QWidget *parentWindow, BrainQuarter *viewer) : QWidget(parentWindow){
	ui.setupUi(this);
	
	this->viewer = viewer;
	//this->parentWindow = (ResourceForm *) parentWindow; //static_cast<QMainWindow *>(parent);
	haveProps = false;
	
	QStringList headers;
	headers << tr("Title") << tr("Description");

	model = new TreeModel(headers, "", this);

	parent = model->getRootItem();
	parent->setViewer(viewer);
	
	haveGeometry = false;
	ui.treeView->setModel(model);
	
	connect(ui.treeView, SIGNAL(clicked(const QModelIndex&)),
			this, SLOT(setPropertyForm(const QModelIndex&)));
	connect(ui.treeView, SIGNAL(clicked(const QModelIndex&)),
			this, SLOT(selectedItem(const QModelIndex&)));
	connect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
			ui.treeView, SLOT(update(const QModelIndex&)));
	
	//sampleTreeSetup(viewer);
}

ResourceForm::~ResourceForm() {
}

void ResourceForm::selectedItem(const QModelIndex &index) {
	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
	item->wasSelected();
}

void ResourceForm::geometrySelected(int childNum) {
	lastGeometry = static_cast<GeometryScene*>(parent->child(childNum));
}

void ResourceForm::setPropertyForm(const QModelIndex & index) {
	// destroy the currently in use form
	if (haveProps) {
		currentPropItem->destroyForm();
	}

	// get a pointer to the selected item
	currentPropItem = static_cast<TreeItem*>(index.internalPointer());

	// only set up form if item has property form
	if (currentPropItem->havePropForm()) {

		// remove the current layout in the property box
		delete ui.propertiesBox->layout();

		if (haveProps) {
			currentPropItem->destroyForm();
		}

		// have the item create its property form
		propWidget = currentPropItem->createForm();
		// create a layout and add the new form to it.
		propLayout = new QVBoxLayout();
		propLayout->addWidget(propWidget);
		ui.propertiesBox->setLayout(propLayout);
		haveProps = true;
	}
}

bool ResourceForm::insertGeometry(QFile &filename) {
	
	parent->insertGeometry(filename);
	lastGeometry = static_cast<GeometryScene*>(parent->child(parent->childCount()-1));
	haveGeometry = true;
	viewer->viewAll();
	somethingChanged();
}	

bool ResourceForm::insertVertstats(QFile &filename) {
	if (haveGeometry) {
		lastGeometry->loadVertstats(filename);
		somethingChanged();
		return true;
	}
	else {
		return false;
	}
}

void ResourceForm::somethingChanged() {
	model->somethingChanged();
	for (int column = 0; column < model->columnCount(); ++column)
        ui.treeView->resizeColumnToContents(column);
}

void ResourceForm::sampleTreeSetup(BrainQuarter *viewer) {

	//parent->insertCone();
	QFile f1("/tmp/mni_icbm_00101_gray_surface_left_81920.obj");
	QFile f2("/tmp/mni_icbm_00101_white_surface_right_81920.obj");
	parent->insertGeometry(f1);
	//insertGeometry(f1);
	GeometryScene *g = static_cast<GeometryScene*>(parent->child(0));
	QFile f3("/tmp/average.vertstats");
	//g->loadVertstats(f3);


	parent->insertGeometry(f2);
	g = static_cast<GeometryScene*>(parent->child(1));
	//g->loadVertstats(f3);
	//g->loadVertstats(f3);
	//parent->insertChildren(parent->childCount(), 1, parent->columnCount() );
	//parent->child(parent->childCount() - 1)->setData(0, "testing");

	
	//viewer->viewAll();
}
