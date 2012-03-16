#ifndef TAGPOINTITEM_H
#define TAGPOINTITEM_H

#include "TreeItem.h"

#include <volume_io.h>

#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCube.h>
#include "Inventor/nodes/SoMaterial.h"
#include "Inventor/nodes/SoMaterialBinding.h"
// #include <Inventor/nodes/SoScale.h>

class tagPointItem : public TreeItem {
public:
    tagPointItem(SoSeparator *root, const QVector<QVariant> &data,
                 ResourceForm *rf, TreeItem *parent = 0,int r=0, int g=255, int b=255,double tagsize =0.1);
    virtual ~tagPointItem();
    void setLocation(double x, double y, double z);
// 	void updateCubeSize(double newsize,double tagsize);
private:
    // the separator onto which the various sphere seps will be attached
    SoSeparator *root;
    // the separator to isolate the translation from other events
    SoSeparator *tagSeparator;
    // the sphere itself
    SoSphere *sphere;
	SoCube *cube;
    // holds the spheres location
    SoTranslation *translation;
	SoMaterial *tagmaterial;
//     SoMaterialBinding *tagmaterialBinding;
	double tagsize;
	

};
#endif // TAGPOINTITEM_H
