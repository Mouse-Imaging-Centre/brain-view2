#include "GeometryScene.h"

// forward declaration
SoSeparator* bic_graphics_file_to_iv( const char* filename );


GeometryScene::GeometryScene(const QVector<QVariant> &data, TreeItem *parent)
	: TreeItem(data, parent){

	scene = new SoSeparator();
	geometry = new GeometryNode(scene, data, this);
}

bool GeometryScene::loadGeometry(QFile &file) {
	root = viewer->getRootSeparator();
	
	root->addChild(scene);
	return geometry->loadObj(file);
	//SoSeparator *geometry = new SoSeparator();
	//geometry = bic_graphics_file_to_iv( file.fileName().toLatin1().data() );
	
	//SoSeparator *root = viewer->getRootSeparator();
	//root->addChild(geometry);
	//viewer->viewAll();
	
	//setData(0, QFileInfo(file).baseName());
}

bool GeometryScene::loadVertstats(QFile &file) {
	
}

GeometryScene::~GeometryScene() {
}
