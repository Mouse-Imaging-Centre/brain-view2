#include "tagPointItem.h"
#include <Inventor/nodes/SoScale.h>

tagPointItem::tagPointItem(SoSeparator *root,
                           const QVector<QVariant> &data,
                           ResourceForm *rf,
                           TreeItem *parent,double tagsize,float r, float g, float b)
    : TreeItem(data, rf, parent) {

    // this class has no property form (for the moment)
    haveProps = false;

    this->root = root;

    // create all the nodes
    tagSeparator = new SoSeparator;
    //sphere = new SoSphere;
	cube= new SoCube;
    //scale = new SoScale;
    translation = new SoTranslation;
	tagmaterial = new SoMaterial;
	//tagcubeVect.append(cube);
	//nodeIdVect.append(cube->getNodeId());
	//tagsize =0.1;

    // attach all necessary bits to the scenegraph
    tagSeparator->addChild(translation);
	tagmaterial->transparency.set1Value(0, 0);
	tagSeparator->addChild(tagmaterial);
	tagmaterial->diffuseColor.setValue(r/255.0,g/255.0,b/255.0);
	cube->width=tagsize;
	cube->height=tagsize;
	cube->depth=tagsize;
	tagSeparator->addChild(cube);
	//tagSeparator->addChild(scale);
	//sphere->radius=tagsize;
	//tagSeparator->addChild(sphere);

    root->addChild(tagSeparator);
}

void tagPointItem::setLocation(double x, double y, double z) {
    translation->translation.setValue(x, y, z);
}

/*
void tagPointItem::updateCubeSize(double newsize,double tagsize){
	scale->scaleFactor={float(newsize/tagsize), float(newsize/tagsize), float(newsize/tagsize)};
	int num= root->getNumChildren();
	qDebug() <<"number of children in the tag Separator:" << num ;
	cube = root->getChild(num-1);
	root->getChild(num-1)->scaleFactor=newsize/tagsize;
	root->getChild(num-1)->height=newsize;
	root->getChild(num-1)->depth=newsize;
	for (int i=0; i<tagcubeVect.size() ; i++){
		tagcubeVect[i]->width=newsize;
		tagcubeVect[i]->height=newsize;
		tagcubeVect[i]->depth=newsize;
	}
}
*/

tagPointItem::~tagPointItem() {
}
