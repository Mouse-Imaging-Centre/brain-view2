#include <QtDebug>
#include "textureFileItem.h"


textureFileItem::textureFileItem( SoSeparator *root, const QVector<QVariant> &data,
		ResourceForm *rf, TreeItem *parent) : TreeItem(data, rf, parent) {


	this->root = root;
	this->rf = rf;

	/** Scene Graph:
	 ** root holds the geometry scene
	 ** The textureSwitch is filled by textureColumns
	 **/

	textureSwitch = new SoSwitch;
	root->insertChild(textureSwitch, 0);

	/* create the vertstats file */
	vertstatsFile = new mniVertstatsFile;

}


textureFileItem::~textureFileItem() {}


void textureFileItem::loadFile(QFile &file) {
	
	/* the name within the TreeItem hierarchy */
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
	int num = 0;
	for (it = headers.begin(); it < headers.end(); it++) { num++;}
	
	
	for (it = headers.begin(); it < headers.end(); it++) {
		textureColumn *newColumn = new textureColumn(textureSwitch, itemData, form, this);
		newColumn->loadTextureColumn(vertstatsFile, QString::fromStdString(*it));
		childItems.insert(childCount(), newColumn);

		connect(this, SIGNAL(setPickedPoint(int)), newColumn, SLOT(setPickInfo(int)));
	}
}

void textureFileItem::pickedPointReceived(int index) { 
	//qDebug() << "index " << index;
	vector <string> headers = vertstatsFile->getDataHeader();
	vector <string>::iterator it;
	int vertscnt =0 ;
	for (it = headers.begin(); it < headers.end(); it++) {
		//qDebug() <<  QString::fromStdString(*it) << ":" << vertstatsFile->getDataColumn(vertscnt)[index];
		//if ( verboseAction->isChecked() )
		rf->emitVerboseSignal();
		if (rf->getVerboseStatus())
			cout << *it << ":" << vertstatsFile->getDataColumn(vertscnt)[index] << "  "<< flush;
		vertscnt++;
	}
	//if ( verboseAction->isChecked() )
	rf->emitVerboseSignal();
	if (rf->getVerboseStatus())
		cout << endl<<endl;
	//qDebug()<< "\n";
	
	emit setPickedPoint(index); 
};














