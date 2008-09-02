#ifndef GEOMETRYNODE_H_
#define GEOMETRYNODE_H_

#include "TreeItem.h"
#include <bicpl.h>

#include "Inventor/nodes/SoCoordinate3.h"
#include "Inventor/nodes/SoNormal.h"
#include "Inventor/nodes/SoNormalBinding.h"
#include "Inventor/nodes/SoMaterial.h"
#include "Inventor/nodes/SoMaterialBinding.h"
#include "Inventor/nodes/SoIndexedFaceSet.h"
#include "ui_geometryProps.h"

class GeometryNode : public TreeItem {
	Q_OBJECT
public:
	GeometryNode(SoSeparator *root, const QVector<QVariant> &data,
			ResourceForm *rf, TreeItem *parent = 0);
	bool loadObj(QFile &file);
	virtual ~GeometryNode();
	QWidget *createForm();
	void destroyForm();
public slots:
	void updateTransparency(double newVal);
private:
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
	// the indices into the coordinates to build the surfaces
	SoIndexedFaceSet *indexedFaceSet;

	bool loadPolygons(const polygons_struct& p);
	Ui::geometryProps ui;
};

#endif /*GEOMETRYNODE_H_*/
