#include "GeometryScene.h"

// forward declaration
SoSeparator* bic_graphics_file_to_iv( const char* filename );


GeometryScene::GeometryScene(const QVector<QVariant> &data, TreeItem *parent)
	: TreeItem(data, parent){
}

void GeometryScene::loadGeometry(QFile &file) {
	SoSeparator *geometry = new SoSeparator();
	geometry = bic_graphics_file_to_iv( file.fileName().toLatin1().data() );
	
	SoSeparator *root = viewer->getRootSeparator();
	root->addChild(geometry);
	viewer->viewAll();
	
	setData(0, QFileInfo(file).baseName());
}

GeometryScene::~GeometryScene() {
}
