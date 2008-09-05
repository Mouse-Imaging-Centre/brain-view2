#include "textureFileItem.h"


textureFileItem::textureFileItem( SoSeparator *root, const QVector<QVariant> &data,
		ResourceForm *rf, TreeItem *parent) : TreeItem(data, rf, parent){) {

	this->root = root;
	
	/* Scene Graph:
	 * root holds the geometry scene
	 * textureComplexity is inserted at the beginning of the geometry scene graph
	 * textureSwitch is inserted after the complexity node.
	 * 		the textureSwitch will hold the textureCoordinates.
	 * a textureBinding node to hold, well, the texture binding.
	 */
	textureComplexity = new SoComplexity;
	textureComplexity->textureQuality = 1;
	root->insertChild(textureComplexity, 0);
	
	textureSwitch = new SoSwitch;
	root->insertChild(textureSwitch, 1);
	
	textureBinding = new SoTextureCoordinateBinding;
	textureBinding->value.setValue(SoTextureCoordinateBinding::PER_VERTEX_INDEXED);
	root->insertChild(textureBinding, 2);
	
	// create the vertstats file
	vertstatsFile = new mniVertstatsFile;
}

textureFileItem::~textureFileItem() {
}

void textureFileItem::loadFile(QFile &file) {
	// the name within the TreeItem hierarchy
	setData(0, QFileInfo(file).baseName());

	vertstatsFile->loadFile( (char *) file.fileName().toLatin1().data());
	
	
}