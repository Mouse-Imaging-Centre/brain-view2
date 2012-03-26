#include "ResourceForm.h"
#include <QtGui>
#include "GeometryScene.h"
// #include "TagForm.h"

#include <iostream>

// //global flag for labeling the object file?
// int flag_label = 0;

ResourceForm::ResourceForm(QWidget *parentWindow, BrainQuarter *viewer) : QWidget(parentWindow){
// 	qDebug() << "Debug. <<ResourceForm::ResourceForm()";
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
	
// 	tagWidget = new TagForm(parentWindow, viewer);
// 	qDebug() << "Debug. <<ResourceForm::TagForm()";
	tagWidget = new QWidget;
	uitag.setupUi(tagWidget);
	uitag.CreateTagPoint->setEnabled ( FALSE );
	uitag.AddTagPoint->setEnabled ( FALSE );
	uitag.SaveTagFile->setEnabled ( FALSE );
	uitag.TagSize->setEnabled ( FALSE );

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

	// create a layout and add the new form to it.
	tagLayout = new QVBoxLayout();
	tagLayout->addWidget(tagWidget);
	ui.Tags->setLayout(tagLayout);
// 	qDebug() << "Debug. >>ResourceForm::TagForm()";	
	
// 	qDebug() << "Debug. >>ResourceForm::createLabelForm()";
	labelWidget = new QWidget;
	uilabel.setupUi(labelWidget);
	
/*	cout << "\n\n\n"<< labelnames.size() << "\n\n\n";
	for (int i=0; i<labelnames.size(); i++){
		qDebug << labelnames.at(i);
		uilabel.LabelVessel->addItem(labelnames.at(i));
	}*/
/*	uilabel.LabelVessel->addItem("No label\0");	
	uilabel.LabelVessel->addItem("Anterior Cerebral Artery\0");	uilabel.LabelVessel->addItem("R. Middle Cerebral Artry\0");	uilabel.LabelVessel->addItem("L. Middle Cerebral Artry\0");	
	uilabel.LabelVessel->addItem("R. Internal Carotid Artery\0");	uilabel.LabelVessel->addItem("L. Internal Carotid Artery\0");	uilabel.LabelVessel->addItem("R. Posterior Comm. Artry\0");	uilabel.LabelVessel->addItem("L. Posterior Comm. Artry\0");	
	uilabel.LabelVessel->addItem("R. Posterior Cereb. Artry\0");	uilabel.LabelVessel->addItem("L. Posterior Cereb. Artry\0");	uilabel.LabelVessel->addItem("R. Superior Cereb. Artery\0");	uilabel.LabelVessel->addItem("L. Superior Cereb. Artery\0");	
	uilabel.LabelVessel->addItem("R. Ant. Inf. Cereb. Artry\0");	uilabel.LabelVessel->addItem("L. Ant. Inf. Cereb. Artry\0");	uilabel.LabelVessel->addItem("Basilar Artery\0");	uilabel.LabelVessel->addItem("Vertebral Artery\0");	
	uilabel.LabelVessel->addItem("R. Internal Audit Artery\0");	uilabel.LabelVessel->addItem("L. Internal Audit Artery\0");
	
	uilabel.LabelVessel->addItem("Superior Saggital Sinus\0");	uilabel.LabelVessel->addItem("Great Cerebral Vein of Galen\0");	uilabel.LabelVessel->addItem("R. Transverse Sinus\0");	uilabel.LabelVessel->addItem("L. Transverse Sinus\0");
	uilabel.LabelVessel->addItem("R. Caudal Rhinal Vein\0");	uilabel.LabelVessel->addItem("L. Caudal Rhinal Vein\0");	uilabel.LabelVessel->addItem("R. Rostral Rhinal Vein\0");	uilabel.LabelVessel->addItem("L. Rostral Rhinal Vein\0");
	uilabel.LabelVessel->addItem("R. Sigmoid Sinus\0");	uilabel.LabelVessel->addItem("L. Sigmoid Sinus\0");	uilabel.LabelVessel->addItem("R. Longitud. Hippo. Vein\0");	uilabel.LabelVessel->addItem("L. Longitud. Hippo. Vein\0");
	uilabel.LabelVessel->addItem("R. Thalamostriate Vein\0");	uilabel.LabelVessel->addItem("L. Thalamostriate Vein\0");	uilabel.LabelVessel->addItem("R. Medial Colicular Vein\0");	uilabel.LabelVessel->addItem("L. Medial Colicular Vein\0");*/
	
// 	uilabel.LabelVessels->setEnabled ( FALSE );
	uilabel.LabelVessel->setEnabled ( FALSE );
	uilabel.SaveLabeledash5->setEnabled ( FALSE );
	uilabel.radiusSlider->setEnabled ( FALSE );
	uilabel.radius->setEnabled ( FALSE );
// 	connect(uilabel.LabelVessels, SIGNAL(clicked()),
// 			this, SLOT(enableSaveLabel()));
	connect(uilabel.LabelVessel, SIGNAL(activated( int )),
			this, SLOT(enableSaveLabel()));			
/*
	connect(uilabel.radiusSlider, SIGNAL(valueChanged(int)),
			lastGeometry, SLOT(updateRadiusTransparency(int)));
	connect(uilabel.LabelVessels, SIGNAL(clicked()),
			lastGeometry, SLOT(userLable()));
	connect(uilabel.SaveLabeledash5, SIGNAL(clicked()),
			lastGeometry, SLOT(saveLabel()));*/
			
	labelLayout = new QVBoxLayout();
	labelLayout->addWidget(labelWidget);
	ui.Labels->setLayout(labelLayout);		
// 	qDebug() << "Debug. <<ResourceForm::createLabelForm()";
	
	connect(ui.treeView, SIGNAL(clicked(const QModelIndex&)),
			this, SLOT(setPropertyForm(const QModelIndex&)));
	connect(ui.treeView, SIGNAL(clicked(const QModelIndex&)),
			this, SLOT(selectedItem(const QModelIndex&)));
	connect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
			ui.treeView, SLOT(update(const QModelIndex&)));
	
	//sampleTreeSetup(viewer);
// 	qDebug() << "Debug. >>ResourceForm::ResourceForm()";
}

ResourceForm::~ResourceForm() {
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

void ResourceForm::enableSaveLabel(){
	uilabel.SaveLabeledash5->setEnabled ( TRUE );
}

void ResourceForm::selectedItem(const QModelIndex &index) {
	qDebug() << "Debug. <<ResourceForm::selectedItem()";
	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        item->wasSelected();
		//viewer->setBackgroundColor 	( QColor(170,85,255.0));
// 		qDebug() << "Debug. >>ResourceForm::selectedItem()";
}

void ResourceForm::updatebckgrndColour(QColor &colour){
	//this->viewer = viewer;
	int r,g,b;
	colour.getRgb(&r,&g,&b);
	std::cout <<"colors rgb are: " << r << "," << g <<","<<b << std::endl;
		viewer->setBackgroundColor 	( QColor(r, g, b));
}		
void ResourceForm::geometrySelected(int childNum) {
	qDebug() << "Debug. <<ResourceForm::geometrySelected()";
	lastGeometry = static_cast<GeometryScene*>(parent->child(childNum));
// 	qDebug() << "Debug. >>ResourceForm::geometrySelected()";
}

void ResourceForm::setPropertyForm(const QModelIndex & index) {
// 	qDebug() << "Debug. <<ResourceForm::setPropertyForm()";
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
// 	qDebug() << "Debug. >>ResourceForm::setPropertyForm()";
}


bool ResourceForm::insertGeometry(QFile &filename) {
	std::cout << "Debug. >>ResourceForm::insertGeometry()" <<std::endl;
// 	GeometryScene* lastGeometry = parent->insertGeometry(filename);
	parent->insertGeometry(filename);
// 	std::cout << "Debug. >>ResourceForm::insertGeometry():lastGeometry" <<std::endl;
	lastGeometry = static_cast<GeometryScene*>(parent->child(parent->childCount()-1));
	
	QFileInfo fileInfo(filename);
    QString ext = fileInfo.suffix();

    if (ext == "h5") {
	    h5childCount.append(parent->childCount()-1);
    }
// 		labelWidget = new QWidget;
// 		uilabel.setupUi(labelWidget);
// 
// 	// 	ui.transparencySpinBox->setValue(*trans);
// 		//emit propertyFormInstantiated(formWidget);
// 	// 	connect(uilabel.radiusSlider, SIGNAL(/*valueChanged*/sliderMoved(double)),
// 	// 			geometry, SLOT(updateRadiusTransparency(double)));
// 		connect(uilabel.LabelVessels, SIGNAL(clicked()),
// 				lastGeometry, SLOT(LabelFlagActivate()));
// 		connect(uilabel.SaveLabeledash5, SIGNAL(clicked()),
// 				lastGeometry, SLOT(SaveLabel()));
// 				
// 		labelLayout = new QVBoxLayout();
// 		labelLayout->addWidget(labelWidget);
// 		ui.Labels->setLayout(labelLayout);
// 	}


// 	connect(uilabel.radiusSlider, SIGNAL(valueChanged(int)),
// 			lastGeometry, SLOT(updateRadiusTransparency(int)));
// 	connect(uilabel.LabelVessels, SIGNAL(clicked()),
// 			lastGeometry, SLOT(userLable()));
// 	connect(uilabel.LabelVessels, SIGNAL(clicked()),
// 			this, SLOT(enableSaveLabel()));		
// 	connect(uilabel.SaveLabeledash5, SIGNAL(clicked()),
// 			lastGeometry, SLOT(saveLabel()));
			
// 	std::cout << "Debug. <<ResourceForm::insertGeometry():lastGeometry" <<std::endl;

	connect(viewer, SIGNAL(pointPicked(int, int, int, int, int,SoType)),
		lastGeometry, SLOT(pickReceived(int, int, int, int, int,SoType )));
	connect (viewer, SIGNAL (pointNotPicked()), lastGeometry, SLOT(noPointReceived()));	
// 	std::cout << "Debug. <<ResourceForm::insertGeometry():connect pointPicked viewer" <<std::endl;

	haveGeometry = true;
// 	std::cout << "Debug. >>ResourceForm::viewAll()" <<std::endl;
	
  
	viewer->viewAll();   ////it goes into QuarterWidget.cpp line 571 QuarterWidget::viewAll(void) and error on line 579 sostatemachine->processEventQueue();
// 	std::cout << "Debug. <<ResourceForm::viewAll()" <<std::endl;
	
	somethingChanged();
// 	std::cout << "Debug. <<ResourceForm::insertGeometry()" <<std::endl;
	
	return true;
}	

// bool ResourceForm::insertH5Geometry(QFile &filename) {
// 	
// 	std::cout << "Debug. >>ResourceForm::insertH5Geometry()" <<std::endl;
// 	GeometryScene* lastGeometry = parent->insertGeometry(filename);
// 	std::cout << "Debug. >>ResourceForm::insertGeometry():lastGeometry" <<std::endl;
// 	//lastGeometry = static_cast<GeometryScene*>(parent->child(parent->childCount()-1));
// 	std::cout << "Debug. <<ResourceForm::insertGeometry():lastGeometry" <<std::endl;
// 
// 	connect(viewer, SIGNAL(pointPicked(int, int, int, int, int)),
// 		lastGeometry, SLOT(pickReceived(int, int, int, int, int)));
// 	std::cout << "Debug. <<ResourceForm::insertGeometry():connect pointPicked viewer" <<std::endl;
// 
// 	haveGeometry = true;
// 	std::cout << "Debug. >>ResourceForm::viewAll()" <<std::endl;
// 	
//   
// 	viewer->viewAll();   ////it goes into QuarterWidget.cpp line 571 QuarterWidget::viewAll(void) and error on line 579 sostatemachine->processEventQueue();
// 	std::cout << "Debug. <<ResourceForm::viewAll()" <<std::endl;
// 	
// 	somethingChanged();
// 	std::cout << "Debug. <<ResourceForm::insertH5Geometry()" <<std::endl;
// 	
// 	return true;
// }


bool ResourceForm::insertTagfile(QFile &filename) {
// 	qDebug() << "Debug. <<ResourceForm::insertTagfile()";
	uitag.TagSize->setEnabled ( TRUE );
    parent->insertTags(filename);
// 	qDebug() << "Debug. >>ResourceForm::insertTagfile()";

	return true;
}

float * ResourceForm::addTagpoint(){
// 	qDebug() << "Debug. <<ResourceForm::addTagpoint()";
	uitag.TagSize->setEnabled ( TRUE );
	float *tagpoint= (float*) malloc(sizeof(float) * 3);
	tagpoint[0]= viewer->pickedtag[0];
	tagpoint[1]= viewer->pickedtag[1];
	tagpoint[2]= viewer->pickedtag[2];
// 	std::cout << "\nCreated tag point is : " << tagpoint[0] <<" , " <<tagpoint[1] << " , " << tagpoint[2] <<std::endl;
	parent->createTag(tagpoint);
   //parent->insertTags(filename);
	
// 	qDebug() << "Debug. >>ResourceForm::addTagpoint()";
	return tagpoint;
}

double ResourceForm::returnTagsize(){
	double tagsize= parent->currentTagSize;
   //parent->insertTags(filename);
	
// 	qDebug() << "Debug. >>ResourceForm::addTagpoint()";
	return tagsize;
}

bool ResourceForm::insertVertstats(QFile &filename) {
// 	qDebug() << "Debug. <<ResourceForm::insertVertstats()";
	if( !filename.exists() )
	{
		qDebug() << "The file" << filename.fileName() << "does not exist.";
		return false;
	}
	if ( haveGeometry ) {
		lastGeometry->loadVertstats(filename);
		somethingChanged();
// 		qDebug() << "Debug. >>ResourceForm::insertVertstats()";
		return true;
	}
	else {
		return false;
	}
}

bool ResourceForm::insertLabelLUfile(QFile &filename) {
	qDebug() << "Debug. <<ResourceForm::insertLabelLUfile()";
	if (!filename.open(QIODevice::ReadOnly | QIODevice::Text)){
		qDebug() << "The file " << filename.fileName() << " can't be opened in readonly mode";
		return false;
	}
	
	//empty the current uilable
// 	for (int i=0; i<uilabel.LabelVessel->count(); i++){
// 		uilabel.LabelVessel->removeItem(i);
// 	}
	uilabel.LabelVessel->clear();
	labelnums.clear();
	labelnames.clear();
	labelred.clear();
	labelgreen.clear();
	labelblue.clear();
	
	
// 	if( !filename.exists() )
// 	{
// 		qDebug() << "The file " << filename.fileName() << "does not exist.";
// 		return false;
// 	}
	while (!filename.atEnd()) {
		QString line = filename.readLine();
		if (line.at(0)!= QChar('#')){
// 			qDebug() << line;
			QStringList list = line.split(";");
			
			if (list.size()>4){
				labelnums.append(list[0].toInt());
				labelnames.append(list[1]);
				labelred.append(list[2].toFloat());
				labelgreen.append(list[3].toFloat());
				labelblue.append(list[4].toFloat());
				qDebug() << list[1];
			}
		}
	}
	// qDebug() << labelnums;
	qDebug() << labelnames;
// 	qDebug() << labelblue;

	filename.close();
	cout << "\n\n\n"<< labelnames.size() << "\n\n\n";

	for (int i=0; i<labelnames.size(); i++){
// 		qDebug() << labelnames.at(i);
		uilabel.LabelVessel->addItem(labelnames.at(i));
	}
	
	if ( haveGeometry ) {
		qDebug() << "have geometry";
		for (int cc=0; cc<h5childCount.size(); cc++){
			lastGeometry = static_cast<GeometryScene*>(parent->child(h5childCount.at(cc)));
			lastGeometry->updateLabelLUColor();
		}
	}
 	return true;

}

void ResourceForm::somethingChanged() {
// 	std::cout << "Debug. >>ResourceForm::somethingChanged()" <<std::endl;
	model->somethingChanged();
	for (int column = 0; column < model->columnCount(); ++column)
        ui.treeView->resizeColumnToContents(column);
// 	std::cout << "Debug. >>ResourceForm::somethingChanged()" <<std::endl;
        
}

void ResourceForm::sampleTreeSetup(BrainQuarter *viewer) {
	qDebug() << "Debug. <<ResourceForm::sampleTreeSetup()";

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
	qDebug() << "Debug. >>ResourceForm::sampleTreeSetup()";
}
