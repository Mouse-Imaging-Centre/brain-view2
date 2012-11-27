#include "tagFileItem.h"
#include "tagPointItem.h"

tagFileItem::tagFileItem(SoSeparator *root,
                         const QVector<QVariant> &data,
                         ResourceForm *rf,
                         TreeItem *parent)
    : TreeItem(data, rf, parent) {

    // no property form yet
    haveProps = false;

    // the root separator from calling function onto which
    // all new bits will be attached.
    this->root = root;
    //this->root=viewer->getRootSeparator();
    //std::cout << "Woohooo!" << std::endl;
    // create all the necessary nodes
    tagSeparator = new SoSeparator;
    scale = new SoScale;
    material = new SoMaterial;
    pickStyle = new SoPickStyle;

    // set the tags to be unpickable for the moment.
    pickStyle->style = SoPickStyle::UNPICKABLE;

    // // initialize some properties - make tags red for the moment
    //material->diffuseColor.setValue(1.0, 0.0, 0.0);

    // attach bits to the scenegraph.
    tagSeparator->addChild(pickStyle);
    tagSeparator->addChild(material);
    tagSeparator->addChild(scale);

    root->addChild(tagSeparator);
    // inittagsize=0.05;	//initialize to the initial value of the tagSize QWidget
}

tagFileItem::~tagFileItem() {
}

bool tagFileItem::loadFile(QFile &file, double tagsize, float r, float g, float b) {
    int n_volumes, n_tag_points;
    double *weights;
    int *structure_ids;
    int *patient_ids;
    // set the name of this tree item
    setData(0, QFileInfo(file).baseName());
    if (input_tag_file((char *)file.fileName().toLatin1().data(),
                       &n_volumes,
                       &n_tag_points,
                       &tags1,
                       &tags2,
                       &weights,
                       &structure_ids,
                       &patient_ids,
                       &labels) != OK) {
        return false;
    }

    // iterate over tags and create tagPointItems
    for (int i=0; i < n_tag_points; i++) {
		//double tagsize = 0.1;
		if  ( (weights[0] > 0) && (weights[0] < 1.0))
				tagsize = weights[0];
		tagPointItem *newTag = new tagPointItem(tagSeparator,
										itemData,
										form,
										this, tagsize,/*0.0,255.0,255.0*/r,g,b);
		newTag->setLocation(tags1[i][0],
						tags1[i][1],
						tags1[i][2]);
		childItems.insert(childCount(), newTag);
    }
    return true;
}

bool tagFileItem::showTag(float *tagpoint, double tagsize, float r, float g, float b){
	tagPointItem *newTag = new tagPointItem(tagSeparator,
                                                itemData,
                                                form,
                                                this, tagsize,/*0.0,255.0,0.0*/r,g,b);
										
        newTag->setLocation(tagpoint[0],
                            tagpoint[1],
                            tagpoint[2]);
		
	/*		newTagVect.append(newTag);			
			std::cout <<"size of newTagVect in create tag: " << newTagVect.size() << std::endl;*/
	//         childItems.insert(childCount(), newTag);
	return true;
}














