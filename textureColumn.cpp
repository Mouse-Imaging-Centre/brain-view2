#include "textureColumn.h"

textureColumn::textureColumn(SoSwitch *root, const QVector <QVariant> &data,
			    ResourceForm *rf, TreeItem *parent)
	: TreeItem(data, rf, parent) {

	low = new float;
	high = new float;
	texture = new SoTextureCoordinate2;
	this->data = new vertexColumn;
	root->addChild(texture);
	root->whichChild = 0;
}

void textureColumn::loadTextureColumn(mniVertstatsFile *file, QString columnName) {
	setData(0, columnName);
	*this->data = file->getDataColumn(columnName.toLatin1().data());
	// determine the low and high
	vertexColumn::iterator it;
	*this->low = *data->begin();
	*this->high = *this->low;
	for (it = data->begin(); it < data->end(); it++) {
		if ( *it > *high )
			*high = *it;
		else if ( *it < *low )
			*low = *it;
	}
	scaleTexture(*low, *high);
}

void textureColumn::scaleTexture(float low, float high) {
	*this->low = low;
	*this->high = high;
	vertexColumn::iterator it;
	int i = 0;
	//SbVec2f *vertValues = texture->point.startEditing();
	for (it = data->begin(); it < data->end(); it++) {
		float newVal = ( *it - *this->low ) / ( *this->high - *this->low);
		if (newVal < 0.0) {
			newVal = 0.0;
		}
		else if (newVal > 1.0) {
			newVal = 1.0;
		}
		//vertValues[i].setValue(0.5, newVal);
		texture->point.set1Value(i, SbVec2f(0.5, newVal));
		i++;
	}
	//texture->point.finishEditing();

}

textureColumn::~textureColumn()
{
}
