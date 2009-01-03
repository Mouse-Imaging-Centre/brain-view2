#ifndef TAGPOINTITEM_H
#define TAGPOINTITEM_H

#include "TreeItem.h"

#include <volume_io.h>

#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoSphere.h>

class tagPointItem : public TreeItem {
public:
    tagPointItem(SoSeparator *root, const QVector<QVariant> &data,
                 ResourceForm *rf, TreeItem *parent = 0);
    virtual ~tagPointItem();
    void setLocation(double x, double y, double z);
private:
    // the separator onto which the various sphere seps will be attached
    SoSeparator *root;
    // the separator to isolate the translation from other events
    SoSeparator *sphereSeparator;
    // the sphere itself
    SoSphere *sphere;
    // holds the spheres location
    SoTranslation *translation;
};
#endif // TAGPOINTITEM_H
