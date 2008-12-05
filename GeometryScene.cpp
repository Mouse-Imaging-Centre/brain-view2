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

void GeometryScene::pickReceived(int index, int id, int x, int y, int z) {
    // check to see whether picked point belongs to this scene.
    if (id == geometry->getID()) {
        // picked point does belong to this scene
        // set the info column in the GUI
        QString info = "Point: ";
        info.append(QString::number(index));
        this->setData(1, info);
        // emit a signal that will be picked up eventually by all the
        // texture columns, causing them to update their GUI info values.
        emit localPointPicked(index);
        // make sure that the GUI will actually be updated
        rf->somethingChanged();
    }
    else {
        // point does not belong to the scene - unset text
        this->setData(1, "");
    }
}

void GeometryScene::wasSelected() {
	rf->setLastGeometry(this);
}

bool GeometryScene::loadVertstats(QFile &file) {
	textureFileItem *v = new textureFileItem(scene, itemData, form, this);
	v->loadFile(file);
        childItems.insert(childCount(), v);
        // make sure that the texture file receives the signal if local point was selected in
        // a ray-picking action.
        connect(this, SIGNAL(localPointPicked(int)), v, SLOT(pickedPointReceived(int)));
}

GeometryScene::~GeometryScene() {
}
