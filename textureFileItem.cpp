#include "textureFileItem.h"


textureFileItem::textureFileItem( SoSeparator *root, const QVector<QVariant> &data,
		ResourceForm *rf, TreeItem *parent) : TreeItem(data, rf, parent) {

	this->root = root;

	/* Scene Graph:
	 * root holds the geometry scene
	 * The textureSwitch is filled by textureColumns
	 */

	textureSwitch = new SoSwitch;
	root->insertChild(textureSwitch, 0);

	// create the vertstats file
	vertstatsFile = new mniVertstatsFile;
}

textureFileItem::~textureFileItem() {
}

void textureFileItem::loadFile(QFile &file) {
	// the name within the TreeItem hierarchy
	setData(0, QFileInfo(file).baseName());

	vertstatsFile->loadFile( (char *) file.fileName().toLatin1().data());
	getAllColumns();


}


/*!
    \fn textureFileItem::getAllColumns()
 */
void textureFileItem::getAllColumns() {
	vector <string> headers = vertstatsFile->getDataHeader();
	vector <string>::iterator it;
	for (it = headers.begin(); it < headers.end(); it++) {
		textureColumn *newColumn = new textureColumn(textureSwitch, itemData, form, this);
		newColumn->loadTextureColumn(vertstatsFile, QString::fromStdString(*it));
                childItems.insert(childCount(), newColumn);

                connect(this, SIGNAL(setPickedPoint(int)), newColumn, SLOT(setPickInfo(int)));
	}
}
