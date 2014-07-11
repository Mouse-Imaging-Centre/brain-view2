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

}

QWidget* GeometryNode::createForm() {
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

	return formWidget;
}

void GeometryNode::updateTransparency(double newVal) {
	material->transparency.set1Value(0, newVal);
	cylmaterial->transparency.set1Value(0, newVal);
	//QColor col(255.0,0.0,0.0);
	//updateColour(col);
}

void GeometryNode::updateCylTransparency(int cyl_num, QVector <SoMaterial *> cylmatVect, float transVal) {
	for (int i=0; i < cyl_num; i++){
		cylmatVect[i]->transparency.set1Value(0, transVal);
	}
	// 	material->transparency.set1Value(0, newVal/100);
	// 	QColor col(255.0,0.0,0.0);
	// 	updateColour(col);
}

void GeometryNode::colourDialog() {
	QColor colour = QColorDialog::getColor();
	if (colour.isValid()) {
		//ui.colourButton->setText(colour.name());
		ui.colourButton->setPalette(QPalette(colour));
		ui.colourButton->setAutoFillBackground(true);
		updateColour(colour);
	}
}

void GeometryNode::bckgrncolourDialog() {
	QColor colour = QColorDialog::getColor();
	if (colour.isValid()) {
		//ui.bckgrndButton->setText(colour.name());
		ui.bckgrndButton->setPalette(QPalette(colour));
		ui.bckgrndButton->setAutoFillBackground(true);
		this->rf = rf;
		rf->updatebckgrndColour(colour);
	}
}

void GeometryNode::updateColour(QColor &colour) {
	int r,g,b;
	colour.getRgb(&r,&g,&b);
	//qDebug() <<"colors rgb are: " << r << "," << g <<","<<b ;
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
}

bool GeometryNode::updateCylinderColour (int cyl_num,QVector <SoMaterial *> cylmatVect, /*QString label*/ int label,float transpar){
// 	qDebug() << "cyl_num = " << cyl_num /*<< " , nodeIDs = " << nodeIDs */<< " , index:getNumChildren() : " << root->getNumChildren();
	struct color_rgb col = colorPicker (label);
	for (int i=0; i<cyl_num; i++){
		cylmatVect[i]->diffuseColor.setValue(col.r/255.0,col.g/255.0,col.b/255.0);
		cylmatVect[i]->transparency.set1Value(0, transpar);
	}
// 	qDebug() << "cylmatVect size : " << cylmatVect.size();
// 	cylmatVect.at(0/*cyl_num*/)->diffuseColor.setValue(1,0,0);
// 	SoNode *currentNode = root->getChild(0);	//root->getChild(cyl_num)//root->getChild(nodeIDs)
// 	qDebug() << "currentNode->getNumChildren() : " << currentNode->getNumChildren();;
// 	SoChildList * children = currentNode->getChildren();
	
// 	cylmaterial/*[cyl_num]*/->diffuseColor.setValue(1,0,0);
// // 	cylmaterial->diffuseColor.set1Value(cyl_num,1,0,0);
}

bool GeometryNode::updatePointColour(int vrtx_indx1, int vrtx_indx2,int segmnet_num, float r, float g, float b) {
	bool labeled_before;
	//check if the vessel segment has been colored before
	const SbColor * clrvalues = material->diffuseColor.getValues(vrtx_indx1);
	SbColor thiscolor = clrvalues[1];
	//qDebug() << "the color value for first index of the selected vessel: " << thiscolor[0] <<","<< thiscolor[1] << "," << thiscolor [2] ;
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
	//qDebug() << "labeled_before = " << labeled_before ;
	return labeled_before;
 }

void GeometryNode::destroyForm() {
	if (formInstantiated) {
		delete formWidget;
		formInstantiated = false;
	}
}


bool GeometryNode::loadObj(QFile &file, int i) {	
	int object_is_line =0; 
	// set the name of this tree item:
	setData(0, QFileInfo(file).baseName());

	// lots of this code came from bicInventor - and parts are still missing
    VIO_File_formats format;
    int num_objects;
    object_struct** object_list;

    // FIXME: fix bicpl for constness on these damn strings!!!
    if ( input_graphics_file( (char*)file.fileName().toLatin1().data(),
    		&format, &num_objects, &object_list ) != VIO_OK ) {
    	return false;
    }

	SoNode* obj=NULL;
	
		
	if (object_list[i]->object_type == POLYGONS) {
		// go ahead and load the polygons.
		obj = loadPolygons(object_list[i]->specific.polygons);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (object_list[i]->object_type ==LINES ) {
		object_is_line=1;
		//go ahead and load the line object.
		obj = loadLines( (object_list[i])->specific.lines );
	}
		

	else {
		qDebug() << "Error: One of the objects is of a type different from Polygons or Lines or Cylinder!";
		qDebug() << object_list[i]->object_type << " is not a supported geometry type!" ;
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
    //qDebug() << "ID: " << indexedFaceSet->getNodeId() ;

    return tmproot;
}

SoSeparator* GeometryNode::loadLines( const lines_struct& l){
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
    //qDebug() << "ID: " << line_set->getNodeId() ;

    return tmproot;
}

GeometryNode::load_Cylinder_output_type GeometryNode::loadCylinder(QFile &file,float vrtxX,float vrtxY,float vrtxZ, float vrtx_radius,float tangentX,float tangentY,float tangentZ, float height, int label){
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
// 		qDebug() << "cylinder ID: " << cylinder_obj->getNodeId(); //returns the node's current unique identification number. 

	

	root->addChild(cylroot);//cylobj

	struct load_Cylinder_output_type output;
	output.nodeid = cylinder_obj->getNodeId();
	//qDebug() << "cylobject node id : " << output.nodeid ;
	output.nodematerial = cylmaterial;
	output.noderadius = vrtx_radius;
	return output;
}

/*
// void GeometryNode::removeCylinder (int nodeid){
// 	//root->getChild(0)->removeChild(nodeid); 
// // 	qDebug() << "root children : " << root->getNumChildren() ;
// //qDebug() << "cylroot children : " << cylroot->getNumChildren() ;
// 	//qDebug() << "parent's children : " << root->getChild(0) ;// 	
	// 	//qDebug() << "parent's children : " << root->getParent()->getNumChildren() ;
// }
*/

GeometryNode::color_rgb GeometryNode::colorPicker (/*QString label*/ int label){
	float r=255.0; float g=255.0; float b=255.0;	//color white

	int lindx = rf->labelnums.indexOf(label);
	if (lindx > -1){
		r = rf->labelred.at(lindx);
		g = rf->labelgreen.at(lindx);
		b = rf->labelblue.at(lindx);
	}
	
	struct color_rgb picked_color;
	picked_color.r = r;
	picked_color.g = g;
	picked_color.b = b;
	return picked_color;	
}

GeometryNode::~GeometryNode()
{
}
