#include "GeometryNode.h"
#include <iostream>

GeometryNode::GeometryNode(SoSeparator *root, const QVector<QVariant> &data,
		ResourceForm *rf, TreeItem *parent)
	: TreeItem(data, rf, parent){

	// this class has a property form
	haveProps = true;

	// the root separator onto which all the bits are to be
	// attached is provided by the creator of this class
	this->root = root;

	// create all of the nodes used to render the surface
	coordinates = new SoCoordinate3;
	normals = new SoNormal;
	normalBinding = new SoNormalBinding;
	material = new SoMaterial;
	materialBinding = new SoMaterialBinding;
	indexedFaceSet = new SoIndexedFaceSet;
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

	}
	return formWidget;
}

void GeometryNode::updateTransparency(double newVal) {
	material->transparency.set1Value(0, newVal);
}

void GeometryNode::updateColour(QColor *colour) {
	int r,g,b;
	colour->getRgb(&r,&g,&b);
	material->diffuseColor.setValue(r/255, g/255, b/255);
}

void GeometryNode::destroyForm() {
	if (formInstantiated) {
		delete formWidget;
		formInstantiated = false;
	}
}


bool GeometryNode::loadObj(QFile &file) {

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
    // only deal with one object for now
    if (num_objects > 1) {
    	return false;
    }
    if (object_list[0]->object_type != POLYGONS) {
    	return false;
    }
    // go ahead and load them thar polygons.
    return loadPolygons(object_list[0]->specific.polygons);
}

bool GeometryNode::loadPolygons(const polygons_struct& p) {

	//SoBaseColor* color = add_color_nodes( root, p.colour_flag, p.colours[0] );

	//SoNormalBinding* normal_binding = new SoNormalBinding;
    //SoNormal* normal = new SoNormal;

    // test materials bit
    //material->diffuseColor.set1Value(0, 1,0,0);
    material->transparency.set1Value(0, 0.5);
    //material->diffuseColor.setValue(1.0, 0, 0);
    root->addChild(material);
    materialBinding->value = SoMaterialBinding::OVERALL;
    root->addChild(materialBinding);

    normals->setName("Normals");

    if ( p.normals ) {
    	normalBinding->value = SoNormalBinding::PER_VERTEX_INDEXED;
    	normals->vector.setNum( p.n_points );
    }
    root->addChild(normalBinding);
    root->addChild(normals);


    //SoCoordinate3* coord = new SoCoordinate3;
    coordinates->setName("Vertices");
    coordinates->point.setNum( p.n_points );

    for( int v = 0; v < p.n_points; ++v ) {
    	coordinates->point.set1Value( v, p.points[v].coords );

    	if ( p.normals ) {
    		normals->vector.set1Value( v, p.normals[v].coords );
    	}

	/*
		if ( p.colour_flag == PER_VERTEX_COLOURS ) {
	    	color->rgb.set1Value( v, float(get_Colour_r_0_1(p.colours[v])),
				     float(get_Colour_g_0_1(p.colours[v])),
				     	float(get_Colour_b_0_1(p.colours[v])) );
	 */
	}

    root->addChild(coordinates);

    //SoIndexedFaceSet* face_set = new SoIndexedFaceSet;
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
    root->addChild(indexedFaceSet);
    return true;
}

GeometryNode::~GeometryNode()
{
}
