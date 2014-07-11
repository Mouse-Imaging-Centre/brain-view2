#ifndef TAGPOINTITEM_H
#define TAGPOINTITEM_H

#include "TreeItem.h"

#define HAVE_MINC2 1

#include <volume_io.h>

#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCube.h>
#include "Inventor/nodes/SoMaterial.h"
#include "Inventor/nodes/SoMaterialBinding.h"

class tagPointItem : public TreeItem {
public:
    tagPointItem(SoSeparator *root, const QVector<QVariant> &data,
                 ResourceForm *rf, TreeItem *parent = 0,double tagsize =0.1,float r=0.0, float g=255.0, float b=255.0);
    virtual ~tagPointItem();
    void setLocation(double x, double y, double z);
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
	double tagsize;
	
};
#endif /* TAGPOINTITEM_H */
