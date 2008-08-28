#ifndef GEOMETRYSCENE_H_
#define GEOMETRYSCENE_H_

#include "TreeItem.h"
#include "Inventor/nodes/SoSeparator.h"

class GeometryScene : public TreeItem {
public:
	GeometryScene(const QVector<QVariant> &data, TreeItem *parent = 0);
	virtual ~GeometryScene();
	void loadGeometry(QFile &file);
};

#endif /*GEOMETRYSCENE_H_*/
