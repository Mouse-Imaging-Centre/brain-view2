#include "GeometryScene.h"

// forward declaration
SoSeparator* bic_graphics_file_to_iv( const char* filename );


GeometryScene::GeometryScene(const QVector<QVariant> &data,
		ResourceForm *rf, TreeItem *parent)
	: TreeItem(data, rf, parent){

	setData(0, "Geometry Scene");
	scene = new SoSeparator();
	geometry = new GeometryNode(scene, data, rf, this);
	childItems.insert(childCount(), geometry);
	this->rf = rf;

}

bool GeometryScene::loadGeometry(QFile &file) {
	root = viewer->getRootSeparator();

	root->addChild(scene);
	return geometry->loadObj(file);

}

void GeometryScene::wasSelected() {
	rf->setLastGeometry(this);
}

bool GeometryScene::loadVertstats(QFile &file) {
	textureFileItem *v = new textureFileItem(scene, itemData, form, this);
	v->loadFile(file);
	childItems.insert(childCount(), v);
}

GeometryScene::~GeometryScene() {
}
