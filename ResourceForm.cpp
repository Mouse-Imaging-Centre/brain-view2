#include "ResourceForm.h"
#include <QtGui>
#include "GeometryScene.h"
// #include "TagForm.h"

#include <iostream>

// //global flag for labeling the object file?
// int flag_label = 0;

ResourceForm::ResourceForm(QWidget *parentWindow, BrainQuarter *viewer) : QWidget(parentWindow){
	ui.setupUi(this);
	
	this->viewer = viewer;
	//this->parentWindow = (ResourceForm *) parentWindow; //static_cast<QMainWindow *>(parent);
	haveProps = false;
	ifVerbose = false;
	
	QStringList headers;
	headers << tr("Title") << tr("Description");

	model = new TreeModel(headers, "", this);

	parent = model->getRootItem();
	parent->setViewer(viewer);
	
	haveGeometry = false;
	ui.treeView->setModel(model);
	
// 	tagWidget = new TagForm(parentWindow, viewer);
	tagWidget = new QWidget;
	uitag.setupUi(tagWidget);
	uitag.CreateTagPoint->setEnabled ( FALSE );
	uitag.AddTagPoint->setEnabled ( FALSE );
	uitag.SaveTagFile->setEnabled ( FALSE );
	uitag.TagSize->setEnabled ( FALSE );
	uitag.TagColour->setEnabled ( FALSE );

	connect(uitag.CreateTagPoint, SIGNAL(clicked()),
				parentWindow, SLOT(createTagFunc()));
	connect(uitag.CreateTagPoint, SIGNAL(clicked()),
				this, SLOT(enableCreateTag()));			
	connect(uitag.AddTagPoint, SIGNAL(clicked()),
				parentWindow, SLOT(addTagFunc()));	
	connect(uitag.AddTagPoint, SIGNAL(clicked()),
				this, SLOT(enableSaveTag()));			
	connect(uitag.SaveTagFile, SIGNAL(clicked()),
				parentWindow, SLOT(saveTagFromDialog()));
	connect (uitag.TagSize, SIGNAL(valueChanged(double)),
				parent, SLOT(updateTagSize(double)));
	connect(uitag.TagSize, SIGNAL(valueChanged(double)),
				this, SLOT(disableAddTag()));			
	connect(uitag.TagColour, SIGNAL(clicked()),
				parent, SLOT(TagcolourDialog()));			

	connect(this, SIGNAL(verboseSignal()),
				parentWindow, SLOT(verboseStatus()));			

	// create a layout and add the new form to it.
	tagLayout = new QVBoxLayout();
	tagLayout->addWidget(tagWidget);
	ui.Tags->setLayout(tagLayout);
	
	labelWidget = new QWidget;
	uilabel.setupUi(labelWidget);
	
 	uilabel.DeleteEdge->setEnabled ( FALSE );
 	uilabel.AddEdge->setEnabled ( FALSE );
	uilabel.label->setEnabled ( FALSE );
	uilabel.LabelVessel->setEnabled ( FALSE );
	uilabel.Saveh5->setEnabled ( FALSE );
	uilabel.Saveash5->setEnabled ( FALSE );
	uilabel.radiusSlider->setEnabled ( FALSE );
	uilabel.radius->setEnabled ( FALSE );
			
	labelLayout = new QVBoxLayout();
	labelLayout->addWidget(labelWidget);
	ui.Vessels->setLayout(labelLayout);		
	
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

void ResourceForm::setVerboseStatus(bool stat){
	ifVerbose = stat;
	viewer->setVerboseStat(stat);
}

bool ResourceForm::getVerboseStatus(){
	return ifVerbose;
}

void ResourceForm::emitVerboseSignal(){
	emit verboseSignal();	
}

void ResourceForm::enableCreateTag(){
	uitag.AddTagPoint->setEnabled ( TRUE );
}

void ResourceForm::enableSaveTag(){
	uitag.SaveTagFile->setEnabled ( TRUE );
}

void ResourceForm::disableAddTag(){
	uitag.AddTagPoint->setEnabled ( FALSE );
}

/*
void ResourceForm::enableSaveLabel(){
	uilabel.Saveh5->setEnabled ( TRUE );
	uilabel.Saveash5->setEnabled ( TRUE );
}
*/

void ResourceForm::selectedItem(const QModelIndex &index) {
	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
	item->wasSelected();
		//viewer->setBackgroundColor 	( QColor(170,85,255.0));
}

void ResourceForm::updatebckgrndColour(QColor &colour){
	//this->viewer = viewer;
	int r,g,b;
	colour.getRgb(&r,&g,&b);
	//qDebug() <<"colors rgb are: " << r << "," << g <<","<<b;
		viewer->setBackgroundColor 	( QColor(r, g, b));
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
// 	GeometryScene* lastGeometry = parent->insertGeometry(filename);
	parent->insertGeometry(filename);
	lastGeometry = static_cast<GeometryScene*>(parent->child(parent->childCount()-1));
	
	QFileInfo fileInfo(filename);
    QString ext = fileInfo.suffix();

    if (ext == "h5") {
	    h5childCount.append(parent->childCount()-1);
    }
  
#if 0 
		labelWidget = new QWidget;
		uilabel.setupUi(labelWidget);

	// 	ui.transparencySpinBox->setValue(*trans);
		//emit propertyFormInstantiated(formWidget);
	// 	connect(uilabel.radiusSlider, SIGNAL(/*valueChanged*/sliderMoved(double)),
	// 			geometry, SLOT(updateRadiusTransparency(double)));
		connect(uilabel.LabelVessels, SIGNAL(clicked()),
				lastGeometry, SLOT(LabelFlagActivate()));
		connect(uilabel.Saveh5, SIGNAL(clicked()),
				lastGeometry, SLOT(SaveLabel()));
				
		labelLayout = new QVBoxLayout();
		labelLayout->addWidget(labelWidget);
		ui.Labels->setLayout(labelLayout);
	}


	connect(uilabel.radiusSlider, SIGNAL(valueChanged(int)),
			lastGeometry, SLOT(updateRadiusTransparency(int)));
	connect(uilabel.LabelVessels, SIGNAL(clicked()),
			lastGeometry, SLOT(userLable()));
	connect(uilabel.LabelVessels, SIGNAL(clicked()),
			this, SLOT(enableSaveLabel()));		
	connect(uilabel.Saveh5, SIGNAL(clicked()),
			lastGeometry, SLOT(saveLabel()));
#endif

	connect(viewer, SIGNAL(pointPicked(int, int,int, float, float, float,SoType)),
		lastGeometry, SLOT(pickReceived(int, int,int, float, float, float,SoType )));
	connect (viewer, SIGNAL (pointNotPicked()), lastGeometry, SLOT(noPointReceived()));	
// 	connect (lastGeometry, SIGNAL (createEdgeSignal()), lastGeometry, SLOT(addNewEdge()));


	haveGeometry = true;
	
  
	viewer->viewAll();   ////it goes into QuarterWidget.cpp line 571 QuarterWidget::viewAll(void) and error on line 579 sostatemachine->processEventQueue();
	
	somethingChanged();
	
	return true;
}	

/*
bool ResourceForm::insertH5Geometry(QFile &filename) {
	GeometryScene* lastGeometry = parent->insertGeometry(filename);
	//lastGeometry = static_cast<GeometryScene*>(parent->child(parent->childCount()-1));

	connect(viewer, SIGNAL(pointPicked(int, int, int, int, int)),
		lastGeometry, SLOT(pickReceived(int, int, int, int, int)));

	haveGeometry = true;
	viewer->viewAll();   ////it goes into QuarterWidget.cpp line 571 QuarterWidget::viewAll(void) and error on line 579 sostatemachine->processEventQueue();

	somethingChanged();
	
	return true;
}
*/

bool ResourceForm::insertTagfile(QFile &filename) {
	uitag.TagSize->setEnabled ( TRUE );
	uitag.TagColour->setEnabled (TRUE);
	parent->insertTags(filename);

	return true;
}

float * ResourceForm::addTagpoint(){
	uitag.TagSize->setEnabled ( TRUE );
	uitag.TagColour->setEnabled ( TRUE );
	float *tagpoint= (float*) malloc(sizeof(float) * 3);
	tagpoint[0]= viewer->pickedtag[0];
	tagpoint[1]= viewer->pickedtag[1];
	tagpoint[2]= viewer->pickedtag[2];
	parent->createTag(tagpoint);
	//free(tagpoint);
   //parent->insertTags(filename);
	
	return tagpoint;
}

double ResourceForm::returnTagsize(){
	double tagsize= parent->currentTagSize;
   //parent->insertTags(filename);
	return tagsize;
}

bool ResourceForm::insertVertstats(QFile &filename) {
	if( !filename.exists() )
	{
		qDebug() << "The file" << filename.fileName() << "does not exist.";
		return false;
	}
	if ( haveGeometry ) {
		lastGeometry->loadVertstats(filename);
		somethingChanged();
		return true;
	}
	else {
		return false;
	}
}

bool ResourceForm::insertLabelLUfile(QFile &filename) {
	if( !filename.exists() )
	{
		qDebug() << "The file " << filename.fileName() << "does not exist.";
		return false;
	}

	if (!filename.open(QIODevice::ReadOnly | QIODevice::Text)){
		qDebug() << "The file " << filename.fileName() << " can't be opened in readonly mode";
		return false;
	}

/*
	//empty the current uilable
	for (int i=0; i<uilabel.LabelVessel->count(); i++){
		uilabel.LabelVessel->removeItem(i);
	}
*/
	uilabel.LabelVessel->clear();
	labelnums.clear();
	labelnames.clear();
	labelred.clear();
	labelgreen.clear();
	labelblue.clear();

	while (!filename.atEnd()) {
		QString line = filename.readLine();
		if (line.at(0)!= QChar('#')){
			QStringList list = line.split(";");
			
			if (list.size()>4){
				labelnums.append(list[0].toInt());
				labelnames.append(list[1]);
				labelred.append(list[2].toFloat());
				labelgreen.append(list[3].toFloat());
				labelblue.append(list[4].toFloat());
			}
		}
	}

	filename.close();

	for (int i=0; i<labelnames.size(); i++){
		uilabel.LabelVessel->addItem(labelnames.at(i));
	}
	
	if ( haveGeometry ) {
		for (int cc=0; cc<h5childCount.size(); cc++){
			lastGeometry = static_cast<GeometryScene*>(parent->child(h5childCount.at(cc)));
			lastGeometry->updateLabelLUColor();
		}
	}
	
// 	labelnums.clear();
// 	labelnames.clear();
// 	labelred.clear();
// 	labelgreen.clear();
// 	labelblue.clear();

 	return true;

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
