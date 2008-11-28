#ifndef TEXTURECOLUMN_H_
#define TEXTURECOLUMN_H_

#include "TreeItem.h"
#include "ResourceForm.h"
#include "mniVertstatsFile.h"

#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoGroup.h>

#include <QtGui>

#include "ui_textureColumnForm.h"

class textureColumn : public TreeItem {
	Q_OBJECT
public:
	textureColumn(SoSwitch *root, const QVector <QVariant> &data,
				  ResourceForm *rf, TreeItem *parent = 0);
	virtual ~textureColumn();
	void loadTextureColumn(mniVertstatsFile *file, QString columnName);
	void scaleTexture(float low, float high);
	void wasSelected();
	QWidget *createForm();
	void destroyForm();

public slots:
	void upperValueChanged(double newVal);
	void lowerValueChanged(double newVal);
	// when new texture selected in combo-box.
	void newTextureSelected(int index);
private:
	// holds the actual texture to be displayed - modified with each rescale
	SoTextureCoordinate2 *texture;
	// holds the original data - will not be modified.
	vertexColumn *data;
	// low and high are used for scaling the column
	float *low;
	float *high;
	float *minValue;
	float *maxValue;
	// holds a pointe to the parents texture switch
	SoGroup *textureGroup;
	// holds the texture complexity node
	SoComplexity *textureComplexity;
	// all the texture columns will be attached to this switch.
	SoSwitch *textureSwitch;
	// the texture coordinate Binding
	SoTextureCoordinateBinding *textureBinding;
	// the actual vertstats file
	mniVertstatsFile *vertstatsFile;
	// the texture image itself
	SoTexture2 *textureImage;
	// a vector holding texture filenames
	QVector<QString> *textureFiles;

	void fillTextureVector();
	
	
	Ui::textureProps ui;

};

#endif /*TEXTURECOLUMN_H_*/
