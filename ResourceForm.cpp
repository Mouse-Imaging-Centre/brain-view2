#include "ResourceForm.h"
#include <QtGui>

ResourceForm::ResourceForm(QWidget *parent) : QWidget(parent){
	ui.setupUi(this);
	sampleTreeSetup();
}

ResourceForm::~ResourceForm() {
}

void ResourceForm::sampleTreeSetup() {
	QStringList headers;
	headers << tr("Title") << tr("Description");

	QFile file("/usr/lib/qt4/examples/itemviews/editabletreemodel/default.txt");
	file.open(QIODevice::ReadOnly);
	TreeModel *model = new TreeModel(headers, file.readAll());
	file.close();

	ui.treeView->setModel(model);
}