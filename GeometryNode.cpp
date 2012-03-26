#include "GeometryNode.h"
#include <iostream>
//#include <qstring.h>



/**
 * Translate BIC obj file to inventor format.
 **/

#include "bicInventor.h"
#include <bicInventor/object.h>
//#include <object.h>

#include <iostream>

#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>

// shape nodes
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoQuadMesh.h>

// property nodes
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include "Inventor/nodes/SoDirectionalLight.h"

#include <Inventor/nodes/SoTransform.h>
#include <bicInventor.h>
// #include "hdf5.h"		//C API
#include "H5Cpp.h"		//C++ API

#include "math.h"
#include <QVector>

// group nodes
#include <Inventor/nodes/SoSeparator.h>


GeometryNode::GeometryNode(SoSeparator *root, const QVector<QVariant> &data,
		ResourceForm *rf, TreeItem *parent)
	: TreeItem(data, rf, parent){

// 	qDebug() << "Debug. >>GeometryNode::GeometryNode()";
	// this class has a property form
	haveProps = true;
	this->rf = rf;
	// the root separator onto which all the bits are to be
	// attached is provided by the creator of this class
	this->root = root;

	// create all of the nodes used to render the surface
	coordinates = new SoCoordinate3;
	normals = new SoNormal;
	normalBinding = new SoNormalBinding;
	material = new SoMaterial;
// 	material->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_ADD);
	materialBinding = new SoMaterialBinding;
	cylmaterial = new SoMaterial;
	cylmaterialBinding = new SoMaterialBinding;
	indexedFaceSet = new SoIndexedFaceSet;
	line_set = new SoIndexedLineSet;
	num_vrtx=-1;
	cylinder_obj= new SoCylinder;
// 	cylinder_tr = new SoTransform;
	cylinder_translation = new SoTranslation;
	cylinder_rotation = new SoRotation;
	//&quaternion_params = new SbRotation;
// 	cylinder_rotationX = new SoRotationXYZ;
// 	cylinder_rotationY = new SoRotationXYZ;	
// 	cylinder_rotationZ = new SoRotationXYZ;
	
	// 	GeometryScene* lastGeometry;
// 	connect(lastGeometry, SIGNAL(pointPickedColor(int, int, int, int, int)),
// 		this, SLOT(updatePointColor(int, int, int, int, int)));

// 	qDebug() << "Debug. <<GeometryNode::GeometryNode()";
}

QWidget* GeometryNode::createForm() {
// 	qDebug() << "Debug. >>GeometryNode::createForm()";
	if (!formInstantiated) {
		formWidget = new QWidget;
		ui.setupUi(formWidget);
		formInstantiated = true;
		const float *trans = material->transparency.getValues(0);
		ui.transparencySpinBox->setValue(*trans);
		//emit propertyFormInstantiated(formWidget);
		connect(ui.transparencySpinBox, SIGNAL(valueChanged(double)),
				this, SLOT(updateTransparency(double)));
		connect(ui.colourButton, SIGNAL(clicked()),
				this, SLOT(colourDialog()));
		connect(ui.bckgrndButton, SIGNAL(clicked()),
				this, SLOT(bckgrncolourDialog()));
	}
// 	qDebug() << "Debug. <<GeometryNode::createForm()";
	return formWidget;
}

void GeometryNode::updateTransparency(double newVal) {
// 	qDebug() << "Debug. >>GeometryNode::updateTransparency()";
	material->transparency.set1Value(0, newVal);
	cylmaterial->transparency.set1Value(0, newVal);
	//QColor col(255.0,0.0,0.0);
	//updateColour(col);
// 	qDebug() << "Debug. <<GeometryNode::updateTransparency()";
}

void GeometryNode::updateCylTransparency(int cyl_num, QVector <SoMaterial *> cylmatVect, float transVal) {
//  	qDebug() << "Debug. >>GeometryNode::updateCylTransparency()";
	cylmatVect[cyl_num]->transparency.set1Value(0, transVal);
// 	material->transparency.set1Value(0, newVal/100);
	//QColor col(255.0,0.0,0.0);
	//updateColour(col);
//  	qDebug() << "Debug. <<GeometryNode::updateCylTransparency()";
}

void GeometryNode::colourDialog() {
// 	qDebug() << "Debug. >>GeometryNode::colourDialog()";
	QColor colour = QColorDialog::getColor();
	if (colour.isValid()) {
		ui.colourButton->setText(colour.name());
		ui.colourButton->setPalette(QPalette(colour));
		ui.colourButton->setAutoFillBackground(true);
		updateColour(colour);
	}
// 	qDebug() << "Debug. <<GeometryNode::colourDialog()";
}

void GeometryNode::bckgrncolourDialog() {
 	//qDebug() << "Debug. >>GeometryNode::bckgrncolourDialog()";
	QColor colour = QColorDialog::getColor();
	if (colour.isValid()) {
		ui.colourButton->setText(colour.name());
		ui.colourButton->setPalette(QPalette(colour));
		ui.colourButton->setAutoFillBackground(true);
		this->rf = rf;
		rf->updatebckgrndColour(colour);
	}
 	//qDebug() << "Debug. <<GeometryNode::bckgrncolourDialog()";
}

void GeometryNode::updateColour(QColor &colour) {
// 	qDebug() << "Debug. >>GeometryNode::updateColour()";
	int r,g,b;
	colour.getRgb(&r,&g,&b);
	std::cout <<"colors rgb are: " << r << "," << g <<","<<b << std::endl;
// 	if (num_vrtx==-1)		//object is polygon with materialBinding=OVERALL
// 	{
		material->diffuseColor.setValue(r/255.0, g/255.0, b/255.0);
		cylmaterial->diffuseColor.setValue(r/255.0, g/255.0, b/255.0);
// 	}
	/*else*/ if (num_vrtx>0)	//object is line with materialBinding=PER_VERTEX_INDEXED
	{
		for (int v=0; v< num_vrtx; v++) {
			material->diffuseColor.set1Value(v, r/255.0, g/255.0, b/255.0);
		} 
	}
// 	qDebug() << "Debug. <<GeometryNode::updateColour()";
}

bool GeometryNode::updateCylinderColour (int cyl_num,QVector <SoMaterial *> cylmatVect, /*QString label*/ int label,float transpar){
// 	qDebug() << "Debug. >>GeometryNode::updateCylinderColour";
// 	std::cout << "cyl_num = " << cyl_num << " , nodeIDs = " << nodeIDs << " , index:getNumChildren() : " << root->getNumChildren() << std::endl;
	struct color_rgb col = colorPicker (label);
	
	cylmatVect[cyl_num]->diffuseColor.setValue(col.r/255.0,col.g/255.0,col.b/255.0);
	cylmatVect[cyl_num]->transparency.set1Value(0, transpar);
// 	std::cout << "cylmatVect size : " << cylmatVect.size() <<std::endl;
// 	cylmatVect.at(0/*cyl_num*/)->diffuseColor.setValue(1,0,0);
// 	SoNode *currentNode = root->getChild(0);	//root->getChild(cyl_num)//root->getChild(nodeIDs)
// 	std::cout << "currentNode->getNumChildren() : " << currentNode->getNumChildren()<< std::endl;;
// 	SoChildList * children = currentNode->getChildren();
	
// 	cylmaterial/*[cyl_num]*/->diffuseColor.setValue(1,0,0);
// // 	cylmaterial->diffuseColor.set1Value(cyl_num,1,0,0);
// 	qDebug() << "Debug. <<GeometryNode::updateCylinderColour";
}

bool GeometryNode::updatePointColour(int vrtx_indx1, int vrtx_indx2,int segmnet_num, float r, float g, float b) {
 	
//   	qDebug() << "Debug. >>GeometryNode::updatePointColour";
	bool labeled_before;
	//check if the vessel segment has been colored before
	const SbColor * clrvalues = material->diffuseColor.getValues(vrtx_indx1);
	SbColor thiscolor = clrvalues[1];
	//std::cout << "the color value for first index of the selected vessel: " << thiscolor[0] <<","<< thiscolor[1] << "," << thiscolor [2] << std::endl;
	if (thiscolor[0]==1 && thiscolor[1]==1 && thiscolor[2]==1)
	{	
		labeled_before=false; 
	}
	else {
		labeled_before=true;//return true if it has been labeled before
	}
	
	for (int v=vrtx_indx1; v< vrtx_indx2; v++) {
			material->diffuseColor.set1Value(v, 1, 0, 0);
		} 
//    	qDebug() << "Debug. <<GeometryNode::updatePointColour";
	//std::cout << "labeled_before = " << labeled_before <<std::endl;
	return labeled_before;
 }

void GeometryNode::destroyForm() {
// 	qDebug() << "Debug. >>GeometryNode::destroyForm()";
	if (formInstantiated) {
		delete formWidget;
		formInstantiated = false;
	}
// 	qDebug() << "Debug. <<GeometryNode::destroyForm()";
}


// bool GeometryNode::loadObj(QFile &filebool GeometryNode::loadObj(QFile &file, int i, bool cylinder_flag,int v,float vrtx_radius,float tangentX,float tangentY,float tangentZ) {
bool GeometryNode::loadObj(QFile &file, int i) {	
	int object_is_line =0; 
	// set the name of this tree item:
	setData(0, QFileInfo(file).baseName());

	// lots of this code came from bicInventor - and parts are still missing
    File_formats format;
    int num_objects;
    object_struct** object_list;

    // FIXME: fix bicpl for constness on these damn strings!!!
    if ( input_graphics_file( (char*)file.fileName().toLatin1().data(),
    		&format, &num_objects, &object_list ) != OK ) {
    	return false;
    }

	SoNode* obj=NULL;
	
		
	if (object_list[i]->object_type == POLYGONS) {
// 		qDebug() << "Debug. >>GeometryNode::loadObj()";
		std::cout <<"object_is_line =" << object_is_line <<std::endl;
		std::cout << "Loading Polygon object number: " << i <<std::endl;
	// go ahead and load the polygons.
		obj = loadPolygons(object_list[i]->specific.polygons);
// 		qDebug() << "Debug. <<GeometryNode::loadObj()";
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (object_list[i]->object_type ==LINES ) {
		qDebug() << "Debug. >>GeometryNode::loadObj()";
		object_is_line=1;
		std::cout <<"object_is_line =" << object_is_line <<std::endl;
		//go ahead and load the line object.
		std::cout << "Loading Line object number: " << i <<std::endl;
		obj = loadLines( (object_list[i])->specific.lines );
		qDebug() << "Debug. <<GeometryNode::loadObj()";
	}
		

	else {
		std::cout << "Error: One of the objects is of a type different from Polygons or Lines or Cylinder!" <<std::endl;
		std::cout << object_list[i]->object_type << " is not a supported geometry type!" << std::endl;
		return false;  ////for now only reads line and polygon objects. 
		//if you need other types of objects look into the bic_graphics_file_to_iv function and try to add the 
		//#include <object.h> where they are declared. object.h is in bicInventor library installed in /micehome/sghanavati/lib which should be added 		
		//to qmake paths and LD_LIBRARY_PATH
	}
	
	if (obj) {
			// light
 	//root->addChild(new SoDirectionalLight);
		root->addChild(obj);
	}
	return true;
}


SoSeparator* GeometryNode::loadPolygons(const polygons_struct& p) {
// 	qDebug() << "Debug. >>GeometryNode::loadPolygons()";

	SoSeparator* tmproot = new SoSeparator;
	
		// light
 	//tmproot->addChild(new SoDirectionalLight);

	material->transparency.set1Value(0, 0.0);
    tmproot->addChild(material);
    materialBinding->value = SoMaterialBinding::OVERALL;
    tmproot->addChild(materialBinding);

    normals->setName("Normals");

    if ( p.normals ) {
    	normalBinding->value = SoNormalBinding::PER_VERTEX_INDEXED;
    	normals->vector.setNum( p.n_points );
    }
    tmproot->addChild(normalBinding);
    tmproot->addChild(normals);


     coordinates->setName("Vertices");
    coordinates->point.setNum( p.n_points );

    for( int v = 0; v < p.n_points; ++v ) {
    	coordinates->point.set1Value( v, p.points[v].coords );

    	if ( p.normals ) {
    		normals->vector.set1Value( v, p.normals[v].coords );
    	}

	}

    tmproot->addChild(coordinates);

    indexedFaceSet->setName("Polygons");

    int fs_index = 0;
    for( int f = 0; f < p.n_items; ++f ) {
    	int f_size = GET_OBJECT_SIZE( p, f );
    	for( int v = 0; v < f_size; ++v ) {
    		int ind = p.indices[POINT_INDEX(p.end_indices,f,v)];
    		indexedFaceSet->coordIndex.set1Value( fs_index++, ind );
    	}
    	// End of facet indicated by index -1
    	indexedFaceSet->coordIndex.set1Value( fs_index++, -1 );
    }
    tmproot->addChild(indexedFaceSet);
    std::cout << "ID: " << indexedFaceSet->getNodeId() << std::endl;

// 	qDebug() << "Debug. <<GeometryNode::loadPolygons()";
    return tmproot;
}

SoSeparator* GeometryNode::loadLines( const lines_struct& l){
//     qDebug() << "Debug. >>GeometryNode::loadLines";
	num_vrtx=l.n_points;
    SoSeparator* tmproot = new SoSeparator;
	
		// light
 	//tmproot->addChild(new SoDirectionalLight);

    material->transparency.set1Value(0, 0.0);
    tmproot->addChild(materialBinding);
	tmproot->addChild(material);
    materialBinding->value = SoMaterialBinding::PER_VERTEX_INDEXED;//;OVERALL

   coordinates->point.setNum( l.n_points );
	

	l.colour_flag == PER_VERTEX_COLOURS;
	
    for( int v = 0; v < l.n_points; ++v ) {
		coordinates->point.set1Value( v, Point_x(l.points[v]),
				   Point_y(l.points[v]),
				   Point_z(l.points[v]) );
				   
		material->diffuseColor.set1Value( v, 1,1,1);

    }



    tmproot->addChild(coordinates);


    int ls_index = 0;
    for( int i = 0; i < l.n_items; ++i ) {
		int i_size = GET_OBJECT_SIZE( l, i );
		for( int v = 0; v < i_size; ++v ) {
			int ind = l.indices[POINT_INDEX(l.end_indices,i,v)];
			line_set->coordIndex.set1Value( ls_index++, ind );
		}
		// End of polyline indicated by index -1
		line_set->coordIndex.set1Value( ls_index++, -1 );
    }

    tmproot->addChild(line_set);
    std::cout << "ID: " << line_set->getNodeId() << std::endl;

// 	qDebug() << "Debug. <<GeometryNode::loadLines()";
    return tmproot;
}

GeometryNode::load_Cylinder_output_type GeometryNode::loadCylinder(QFile &file,float vrtxX,float vrtxY,float vrtxZ, float vrtx_radius,float tangentX,float tangentY,float tangentZ, float height,int num,/*QString label*/ int label){
	// set the name of this tree item:
	setData(0, QFileInfo(file).baseName());

	SoSeparator* cylroot = new SoSeparator;
	
			// light
 	//cylroot->addChild(new SoDirectionalLight);


	
	cylinder_translation->translation.setValue(vrtxX,vrtxY,vrtxZ);
	
	SbRotation rot(SbVec3f(0, 1, 0),SbVec3f(tangentX, tangentY, tangentZ));
	SbVec3f axis;
	float angle;
	rot.getValue(axis, angle);

	cylinder_rotation->rotation.setValue(axis, angle);
	
	cylroot->addChild(cylinder_translation);   // then translate the cylinder to the right vertices
	cylroot->addChild(cylinder_rotation);		//first rotate by axis-angle from SbRotation between vector (0,0,1) of default cylinder to tangent vector of vertex

    cylmaterial->transparency.set1Value(0, 0.0);
 
    cylroot->addChild(cylmaterial);
    cylmaterialBinding->value = SoMaterialBinding::OVERALL;		//;PER_VERTEX_INDEXED
    cylroot->addChild(cylmaterialBinding);
	
	struct color_rgb col = colorPicker (label);
	
	cylmaterial->diffuseColor.setValue(col.r/255.0,col.g/255.0,col.b/255.0);

	cylinder_obj->radius=vrtx_radius;//get it from db file
	cylinder_obj->height =height;//? half of the (distance from edge before+ distance from edge ahead: if 1 edge:half of dist or more than 2 edges branch=>half of dist of fist edge )
	cylinder_obj->SIDES;

	cylroot->addChild(cylinder_obj);
// 		std::cout << "cylinder ID: " << cylinder_obj->getNodeId() <<std::endl; //returns the node's current unique identification number. 

	

	root->addChild(cylroot);//cylobj
// 	std::cout << "root children : " << root->getNumChildren() << std::endl;
// 	std::cout << "cylroot children : " << cylroot->getNumChildren() << std::endl;
	//std::cout << "parent's children : " << root->getParent()->getNumChildren() << std::endl;

	struct load_Cylinder_output_type output;
	output.nodeid = cylinder_obj->getNodeId();
	output.nodematerial = cylmaterial;
	output.noderadius = vrtx_radius;
	return output;
}

GeometryNode::color_rgb GeometryNode::colorPicker (/*QString label*/ int label){
	float r=255.0; float g=255.0; float b=255.0;	//color white

	int lindx = rf->labelnums.indexOf(label);
	if (lindx > -1){
		r = rf->labelred.at(lindx);
		g = rf->labelgreen.at(lindx);
		b = rf->labelblue.at(lindx);
	}
// 	else
// 		qDebug() << "GeometryNode::colorPicker " << label << " doesn't exist in config file";


// 	//switch (label) {
// 			if ( label == 0){ //"None":
// 			r = 255.0; g = 255.0; b = 255.0; }	//white
// 			
// 		else if ( label == 35){ //"ACA":
// 			r = 255.0; g = 0.0; b = 0.0; }		//red
// 		else if ( label == 191){ //"MCA right":
// 			r = 255.0; g = 255.0; b = 150.0; }	//yellow
// 		else if ( label == 190){ //"MCA left":
// 			r = 255.0; g = 255.0; b = 0.0; }
// 		else if ( label == 2){ //"Internal Carotid artery right":	//blue
// 			r = 100.0; g = 170.0; b = 255.0; }
// 		else if ( label == 43){ //"Internal Carotid artery left":	
// 			r = 0.0; g = 170.0; b = 255.0; }
// 		else if ( label == 200){ //"Posterior Comm. artery right":	//purple
// 			r = 170.0; g = 0.0; b = 255.0; }
// 		else if ( label == 9){ //"Posterior Comm. artery left":
// 			r = 140.0; g = 0.0; b = 255.0; }
// 		else if ( label == 8){ //"PCA right":						//pink
// 			r = 255.0; g = 170.0; b = 255.0; }
// 		else if ( label == 5){ //"PCA left":
// 			r = 255.0; g = 85.0; b = 127.0; }
// 		else if ( label == 68){ //"SCA right":						//orange
// 			r = 255.0; g = 170.0; b = 127.0; }
// 		else if ( label == 227){ //"SCA left":
// 			r = 255.0; g = 100.0; b = 0.0; }
// 		else if ( label == 46){ //"AICA right":						//dark green
// 			r = 0.0; g = 85.0; b = 0.0; }
// 		else if ( label == 12){ //"AICA left":
// 			r = 0.0; g = 85.0; b = 170.0; }
// 		else if ( label == 196){ //"Basilar artery":					//light blue
// 			r = 85.0; g = 255.0; b = 255.0; }
// 		else if ( label == 7){ //"Vertebral artery":				//light green
// 			r = 85.0; g = 255.0; b = 0.0; }
// 		else if ( label == 49){ //"Internal Auditory artery right":
// 			r = 100.0; g = 100.0; b = 150.0; }
// 		else if ( label == 45){ //"Internal Auditory artery left":
// 			r = 120.0; g = 120.0; b = 180.0; }
// 
// 			
// 		else if ( label == 11){ //"Superior Saggital Sinus":			//orange
// 			r = 255.0; g = 85.0; b = 0.0; }
// 		else if ( label == 6){ //"Great Cerebral Vein of Galen":	//blue
// 			r = 0.0; g = 0.0; b = 255.0; }
// 		else if ( label == 30){ //"Transverse Sinus right":			//green
// 			r = 0.0; g = 170.0; b = 0.0; }
// 		else if ( label == 246){ //"Transverse Sinus left":
// 			r = 0.0; g = 255.0; b = 0.0; }
// 		else if ( label == 192){ //"Caudal Rhinal Vein right":		//pink purple
// 			r = 255.0; g = 170.0; b = 255.0; }
// 		else if ( label == 34){ //"Caudal Rhinal Vein left":
// 			r = 255.0; g = 85.0; b = 255.0; }
// 		else if ( label == 20){ //"Rostral Rhinal Vein right":		//red brown
// 			r = 170.0; g = 0.0; b = 0.0; }
// 		else if ( label == 21){ //"Rostral Rhinal Vein left":
// 			r = 170.; g = 85.0; b = 0.0; }
// 		else if ( label == 101){ //"Sigmoid Sinus right":				//yellow green
// 			r = 170.0; g = 170.0; b = 0.0; }
// 		else if ( label == 24){ //"Sigmoid Sinus left":
// 			r = 170.0; g = 170.0; b = 100.0; }
// 		else if ( label == 58){ //"Longitudinal Hippocampal Vein right":	//blue grey
// 			r = 170.0; g = 170.0; b = 255.0; }
// 		else if ( label == 57){ //"Longitudinal Hippocampal Vein left":
// 			r = 111.0; g = 127.0; b = 150.0; }
// 		else if ( label == 56){ //"Thalamostriate Vein right":			//dark pink
// 			r = 255.0; g = 0.0; b = 191.0; }
// 		else if ( label == 54){ //"Thalamostriate Vein left":
// 			r = 170.0; g = 85.0; b = 127.0; }		
// 		else if ( label == 1){ //"Medial Collicular Vein right":
// 			r = 85.0; g = 85.0; b = 0.0; }			
// 		else if ( label == 16){ //"Medial Collicular Vein left":
// 			r = 145.0; g = 145.0; b = 0.0;}
// 		else if ( label == 170){ //#unknown sinus/vein:			//dark purple
// 			r = 85.0; g = 0.0; b = 127.0; }
// 		else if ( label == 171){ //#"Lateral collicular Vein right":      //dark purple
// 			r = 85.0; g = 0.0; b = 170.0; }		
// 		else if ( label == 250){ //#unknown sinus/vein left":			//dark brown
// 			r = 85.0; g = 0.0; b = 0.0; }
// 		else if ( label == 251){ //##unknown sinues/vein right":      //dark pink-purple
// 			r = 170.0; g = 0.0; b = 127.0; }		

		
		
	//}
	

	
	struct color_rgb picked_color;
	picked_color.r = r;
	picked_color.g = g;
	picked_color.b = b;
	return picked_color;	
}

GeometryNode::~GeometryNode()
{
}
