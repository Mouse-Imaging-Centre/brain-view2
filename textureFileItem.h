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
    void getAllColumns();
private:
	void createColumn();
	// holds the geometry separator to which the Switch node will be attached.
	SoSeparator *root;
	// holds the texture complexity node
	SoComplexity *textureComplexity;
	// all the texture columns will be attached to this switch.
	SoSwitch *textureSwitch;
	// the texture coordinate Binding
	SoTextureCoordinateBinding *textureBinding;
	// the actual vertstats file
	mniVertstatsFile *vertstatsFile;
	// holds all of the texture Columns;
	//QVector <textureColumn*> *columns;
};

#endif /*TEXTUREFILEITEM_H_*/
