#ifndef TEXTURECOLUMN_H_
#define TEXTURECOLUMN_H_

#include "TreeItem.h"
#include "ResourceForm.h"
#include "mniVertstatsFile.h"

#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoSwitch.h>

#include <QtGui>

class textureColumn : public TreeItem {
	Q_OBJECT
public:
	textureColumn(SoSwitch *root, const QVector <QVariant> &data, 
				  ResourceForm *rf, TreeItem *parent = 0);
	virtual ~textureColumn();
	void loadTextureColumn(mniVertstatsFile *file, QString columnName);
	void scaleTexture(float low, float high);
private:
	// holds the actual texture to be displayed - modified with each rescale
	SoTextureCoordinate2 *texture;
	// holds the original data - will not be modified.
	vertexColumn *data;
	float *low;
	float *high;

};

#endif /*TEXTURECOLUMN_H_*/
