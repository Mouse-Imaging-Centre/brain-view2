#ifndef GEOMETRYNODE_H_
#define GEOMETRYNODE_H_

#include "TreeItem.h"
#include <bicpl.h>

#include <Inventor/nodes/SoBaseColor.h>
#include "Inventor/nodes/SoCoordinate3.h"
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
// #include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoRotation.h>
#include "Inventor/nodes/SoNormal.h"
#include "Inventor/nodes/SoNormalBinding.h"
#include "Inventor/nodes/SoMaterial.h"
#include "Inventor/nodes/SoMaterialBinding.h"
#include "Inventor/nodes/SoIndexedFaceSet.h"
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include "ui_geometryProps.h"
#include <bicInventor.h>
#include <bicInventor/object.h>
#include <Inventor/nodes/SoCylinder.h>
// #include "hdf5.h"		//C API
#include "H5Cpp.h"		//C++ API
#include <QVector>

class GeometryNode : public TreeItem {
	Q_OBJECT
public:
	GeometryNode(SoSeparator *root, const QVector<QVariant> &data,
			ResourceForm *rf, TreeItem *parent = 0);
	// loads a BIC obj format file (currently only supports polygons and lines)
// 	bool loadObj(QFile &file, int i, bool cylinder_flag, int v=0,float vrtx_radius=0.0,float tangentX=0.0,float tangentY=0.0,float tangentZ=1.0);
	bool loadObj(QFile &file, int i);
	virtual ~GeometryNode();
	QWidget *createForm();
	void destroyForm();
    void updateColour(QColor &colour);	
    int getID() { return indexedFaceSet->getNodeId(); };
	bool updateCylinderColour (int cyl_num,QVector <SoMaterial *> cylmatVect, /*QString label*/ int label,float transpar=0.5);
	bool updatePointColour(int vrtx_indx1, int vrtx_indx2,int segmnet_num, float r, float g, float b);
// 	void CylinderMaterialInit ( int cyl_nums);
	struct load_Cylinder_output_type {
		int nodeid;
		SoMaterial *nodematerial;
		float noderadius;
// 		load_Cylinder_output_type : nodematerial(new SoMaterial) {}
	};	
	
	struct load_Cylinder_output_type loadCylinder( QFile &file,float vrtxX,float vrtxY,float vrtxZ, float vrtx_radius,float tangentX,float tangentY,float tangentZ, float height, int num, /*QString label*/int label);

public slots:
	void updateTransparency(double newVal);
// 	// pops up a dialog to allow the user to chose a colour for the surface
	void colourDialog();
	void bckgrncolourDialog();
	void updateCylTransparency(int cyl_num, QVector <SoMaterial *> cylmatVect, float transVal);
// 	void updatePointColour(int indx1, int indx2, float r, float g, float b);
	
private:
		//Sahar added for reading multiple objects in the same file:
		//SoCoordinate3 *geomCoordinates;		//Sahar added for reading line object:
		// The root to the object scene graph
		//SoSeparator *rootObj;
		// The node that holds the geometry
		//SoSeparator *geometry;
		//SoMaterialBinding *materialBinding;
		//SoBaseColor *colour;

	struct color_rgb {
		float r;
		float g;
		float b;
	};
	
	struct color_rgb colorPicker (/*QString label*/int label);
	
	ResourceForm *rf;
	// holds all the scene elements
	SoSeparator *root;
	// the coordinates of the points in the surface
	SoCoordinate3 *coordinates;
	// the surface normals
	SoNormal *normals;
	// the binding of the surface normals
	SoNormalBinding *normalBinding;
	// the material for the surface
	SoMaterial *material;
	// the material binding
    SoMaterialBinding *materialBinding;
// 	SoMaterial **cylmaterial;
	SoMaterial *cylmaterial;
// 	QVector <SoMaterial *> cylmatVect;
	// the material binding
    SoMaterialBinding *cylmaterialBinding;
	// the transformation for transofrming the cylinders
// 	SoTransform* cylinder_tr;
	SoTranslation *cylinder_translation;
	SoRotation *cylinder_rotation;
// 	SbRotation quaternion_params;
// 	SoRotationXYZ *cylinder_rotationX;
// 	SoRotationXYZ *cylinder_rotationY;
// 	SoRotationXYZ *cylinder_rotationZ;
	
	// the indices into the coordinates to build the surfaces
	SoIndexedFaceSet *indexedFaceSet;
	// the indices into the coordinates to build the lines
	SoIndexedLineSet* line_set;
	//the total number of vertex in line object
	int num_vrtx;
	//for coloring line object
	//SoVertexProperty *clrVertexProperty;
	SoCylinder* cylinder_obj; 

	SoSeparator* loadLines(const lines_struct& l);	
	SoSeparator* loadPolygons(const polygons_struct& p);
/*	SoSeparator* loadCylinder( const lines_struct& l, bool cylinder_flag,int v, float vrtx_radius,float tangentX,float tangentY,float tangentZ );*/
	Ui::geometryProps ui;
};

#endif /*GEOMETRYNODE_H_*/
