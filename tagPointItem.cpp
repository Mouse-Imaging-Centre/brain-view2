#include "tagPointItem.h"
#include <Inventor/nodes/SoScale.h>

tagPointItem::tagPointItem(SoSeparator *root,
                           const QVector<QVariant> &data,
                           ResourceForm *rf,
                           TreeItem *parent,int r, int g, int b,double tagsize)
    : TreeItem(data, rf, parent) {

    // this class has no property form (for the moment)
    haveProps = false;

    this->root = root;

    // create all the nodes
    tagSeparator = new SoSeparator;
	cube= new SoCube;
    translation = new SoTranslation;
	tagmaterial = new SoMaterial;


    // attach all necessary bits to the scenegraph
    tagSeparator->addChild(translation);
	tagmaterial->transparency.set1Value(0, 0);
	tagSeparator->addChild(tagmaterial);
	tagmaterial->diffuseColor.setValue(r/255,g/255,b/255);
	cube->width=tagsize;
	cube->height=tagsize;
	cube->depth=tagsize;
	tagSeparator->addChild(cube);


    root->addChild(tagSeparator);
}

void tagPointItem::setLocation(double x, double y, double z) {
    translation->translation.setValue(x, y, z);
}


tagPointItem::~tagPointItem() {
}
