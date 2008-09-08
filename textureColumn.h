#ifndef TEXTURECOLUMN_H_
#define TEXTURECOLUMN_H_

#include "TreeItem.h"
#inlcude "mniVertstatsFile.h"

class textureColumn : public TreeItem
{
public:
	textureColumn(SoSwitch *root, const QVector <QVariant> &data, 
		      ResourceForm *rf, TreeItem *parent);
	virtual ~textureColumn();
	void loadTextureColumn(mniVertstatsFile *file, string columnName);
	void scaleTexture(float min, float max);
private:
	// holds the actual texture to be displayed - modified with each rescale
	SoTextureCoordinate2 *texture;
	// holds the original data - will not be modified.
	vertexColumn *data;
	float *min;
	float *max;

};

#endif /*TEXTURECOLUMN_H_*/
