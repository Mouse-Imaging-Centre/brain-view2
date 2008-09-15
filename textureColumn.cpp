#include "textureColumn.h"

textureColumn::textureColumn(SoSwitch *root, const QVector <QVariant> &data,
			    ResourceForm *rf, TreeItem *parent)
	: TreeItem(data, rf, parent) {

	low = new float;
	high = new float;
	textureGroup = new SoGroup;

	textureSwitch = root;
	root->addChild(textureGroup);

	textureComplexity = new SoComplexity;
	textureComplexity->textureQuality = 1;
	textureGroup->addChild(textureComplexity);

	textureImage = new SoTexture2;
	textureImage->filename.setValue("/tmp/spectral.png");
	textureImage->wrapT = SoTexture2::CLAMP;
	textureImage->wrapS = SoTexture2::CLAMP;
	textureGroup->addChild(textureImage);

	texture = new SoTextureCoordinate2;
	this->data = new vertexColumn;
	textureGroup->addChild(texture);

	textureBinding = new SoTextureCoordinateBinding;
	textureBinding->value.setValue(SoTextureCoordinateBinding::PER_VERTEX_INDEXED);
	textureGroup->addChild(textureBinding);

	//root->whichChild = 0;
}

void textureColumn::wasSelected() {
	textureSwitch->whichChild = this->childNumber();
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
