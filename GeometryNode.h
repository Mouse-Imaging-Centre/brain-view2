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

class GeometryNode : public TreeItem
{
public:
	GeometryNode(SoSeparator *root, const QVector<QVariant> &data, TreeItem *parent = 0);
	bool loadObj(QFile &file);
	virtual ~GeometryNode();
private:
	SoSeparator *root;
	SoCoordinate3 *coordinates;
	SoNormal *normals;
	SoNormalBinding *normalBinding;
	SoMaterial *material;
	SoMaterialBinding *materialBinding;
	SoIndexedFaceSet *indexedFaceSet;
	
	bool loadPolygons(const polygons_struct& p);
};

#endif /*GEOMETRYNODE_H_*/
