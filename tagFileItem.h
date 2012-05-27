#ifndef TAGFILEITEM_H
#define TAGFILEITEM_H

#include "TreeItem.h"
#include "tagPointItem.h"

#include <volume_io.h>

#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoScale.h>

class tagFileItem : public TreeItem {
public:
    tagFileItem(SoSeparator *root, const QVector<QVariant> &data,
                ResourceForm *rf, TreeItem *parent = 0);
    bool loadFile(QFile &file);
	bool showTag(float *tagpoint, double tagsize);
// 	void updateSize(double newsize, double tagsize);
// 	double inittagsize;  
	virtual ~tagFileItem();
private:
    // function to create a sphere for every tag item
    //void createSpheresFromTags();
    // first set of tags
    double **tags1;
    // second set of tags (if present in tag file)
    double **tags2;
    // labels of tags
    STRING *labels;
    // root separator from calling function
    SoSeparator *root;
    // scale applied to all tags
    SoScale *scale;
    // holds all tags
    SoSeparator *tagSeparator;
    // material properties of all tags
    SoMaterial *material;
    // pick style - whether following nodes are pickage.
    SoPickStyle *pickStyle;
	
};

#endif // TAGFILEITEM_H
