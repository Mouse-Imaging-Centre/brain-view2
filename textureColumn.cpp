#include "textureColumn.h"

textureColumn::textureColumn(SoSwitch *root, const QVector <QVariant> &data, 
			    ResourceForm *rf, TreeItem *parent) 
	: TreeItem(rf, data, parent) {
	
	min = new float;
	max = new float;
	texture = new SoTextureCoordinate2;
	root->addChild(texture);
}

void textureColumn::loadTextureColumn(mniVerstatsFile *file, QString columnName) {
	setData(0, columnName);
	*this->data = file->getDataColumn(columnName.toLatin1().data());
	// determine the min and max
	vertexColumn::iterator it;
	*this->min = *data.begin();
	*this->max = *this->min;
	for (it = *data.begin(); it < *data.end(); it++) {
		if ( *it > *max )
			*max = *it;
		else if ( *it < *min )
			*min = *it;
	}
	scaleTexture(*min, *max);
}

void textureColumn::scaleTexture(float min, float max) {
	vertexColumn::iterator it;
	int i = 0;
	SbVec2f *vertValues = texture->point.startEditing();
	for (it = data->begin(); it < data->end(); it++) {
		float newVal = ( *it - *min ) / ( *max - *min);
		if (newVal < 0.0) {
			newVal = 0.0;
		}
		else if (newVal > 1.0) {
			newVal = 1.0;
		}
		vertValues[i].setValue(0.5, newVal);
		i++;
	}
	texture->point.finishEditing();
	
}

textureColumn::~textureColumn()
{
}
