#include <QtDebug>
#include "textureFileItem.h"


textureFileItem::textureFileItem( SoSeparator *root, const QVector<QVariant> &data,
		ResourceForm *rf, TreeItem *parent) : TreeItem(data, rf, parent) {

	qDebug() << "Debug. >>textureFileItem::textureFileItem()";

	this->root = root;

	/* Scene Graph:
	 * root holds the geometry scene
	 * The textureSwitch is filled by textureColumns
	 */

	textureSwitch = new SoSwitch;
	root->insertChild(textureSwitch, 0);

	// create the vertstats file
	vertstatsFile = new mniVertstatsFile;

	qDebug() << "Debug. <<textureFileItem::textureFileItem()";
}


textureFileItem::~textureFileItem() {}


void textureFileItem::loadFile(QFile &file) {
	qDebug() << "Debug. >>textureFileItem::loadFile()";
	
	// the name within the TreeItem hierarchy
	setData(0, QFileInfo(file).baseName());

	vertstatsFile->loadFile( (char *) file.fileName().toLatin1().data());
	getAllColumns();
	qDebug() << "Debug. <<textureFileItem::loadFile()";
}


/*!
    \fn textureFileItem::getAllColumns()
 */
void textureFileItem::getAllColumns() {
	qDebug() << "Debug. >>textureFileItem::getAllColumns()";

	vector <string> headers = vertstatsFile->getDataHeader();
	vector <string>::iterator it;
	for (it = headers.begin(); it < headers.end(); it++) {
		textureColumn *newColumn = new textureColumn(textureSwitch, itemData, form, this);
		newColumn->loadTextureColumn(vertstatsFile, QString::fromStdString(*it));
		childItems.insert(childCount(), newColumn);

		connect(this, SIGNAL(setPickedPoint(int)), newColumn, SLOT(setPickInfo(int)));
	}
	qDebug() << "Debug. <<textureFileItem::getAllColumns()";
}
















