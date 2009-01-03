#include "tagPointItem.h"

tagPointItem::tagPointItem(SoSeparator *root,
                           const QVector<QVariant> &data,
                           ResourceForm *rf,
                           TreeItem *parent)
    : TreeItem(data, rf, parent) {

    // this class has no property form (for the moment)
    haveProps = false;

    this->root = root;

    // create all the nodes
    sphereSeparator = new SoSeparator;
    sphere = new SoSphere;
    translation = new SoTranslation;

    // attach all necessary bits to the scenegraph
    sphereSeparator->addChild(translation);
    sphereSeparator->addChild(sphere);

    root->addChild(sphereSeparator);
}

void tagPointItem::setLocation(double x, double y, double z) {
    translation->translation.setValue(x, y, z);
}

tagPointItem::~tagPointItem() {
}
