#include "textureFileItem.h"


textureFileItem::textureFileItem( SoSeparator *root, const QVector<QVariant> &data,
		ResourceForm *rf, TreeItem *parent) : TreeItem(data, rf, parent){) {

	this->root = root;
	
	// create the texture Switch, attach it to the beginning of the root separator
	textureSwitch = new SoSwitch;
	root->insertChild(textureSwitch, 0);
	
	// create the vertstats file
	vertstatsFile = new mniVertstatsFile;
}

textureFileItem::~textureFileItem() {
}

void textureFileItem::loadFile(QFile &file) {
	vertstatsFile->loadFile( (char *) file.fileName().toLatin1().data());
	
	
}