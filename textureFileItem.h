#ifndef TEXTUREFILEITEM_H_
#define TEXTUREFILEITEM_H_

#include "TreeItem.h"
#include "textureColumn.h"

#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/nodes/SoTexture2.h>

#include <mniVertstatsFile.h>

class textureFileItem : public TreeItem {
    //Q_OBJECT
public:
    textureFileItem(SoSeparator *root, const QVector<QVariant> &data,
                    ResourceForm *rf, TreeItem *parent = 0);
    void loadFile(QFile &file);
    virtual ~textureFileItem();
    void getAllColumns();
//signals:
  //  void setPickedPoint(int index);
public slots:
    // a simple pass-through slot - when the geometryScene as decided that the
    // most recent pick action was on the local bit of geometry, it sends a signal
    // with the index which is picked up here. This re-emits a signal which is in
    // turn picked up by the textureColumns, who update their info.
    void pickedPointReceived(int index) { emit setPickedPoint(index); };
private:
    void createColumn();
    // holds the geometry separator to which the Switch node will be attached.
    SoSeparator *root;
    // all the texture columns will be attached to this switch.
    SoSwitch *textureSwitch;
    // the actual vertstats file
    mniVertstatsFile *vertstatsFile;

};

#endif /*TEXTUREFILEITEM_H_*/
