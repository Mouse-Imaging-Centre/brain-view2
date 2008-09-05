#ifndef TEXTUREFILEITEM_H_
#define TEXTUREFILEITEM_H_

#include "TreeItem.h"

class textureFileItem : public TreeItem
{
public:
	textureFileItem(SoSeparator *root, const QVector<QVariant> &data,
    				ResourceForm *rf, TreeItem *parent = 0);
	void loadFile(QFile &file);
	virtual ~textureFileItem();
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
