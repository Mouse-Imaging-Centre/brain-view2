/****************************************************************************
** Part of this file was copied from the Qt4 examples, and so is still ...
** Copyright (C) 2005-2008 Trolltech ASA. All rights reserved.
**

    treeitem.cpp
    A container for items of data supplied by the simple tree model.
*/

#include <QStringList>

#include "TreeItem.h"
#include "GeometryScene.h"
#include "tagFileItem.h"



//! [0]
TreeItem::TreeItem(const QVector<QVariant> &data,
		ResourceForm *rf, TreeItem *parent) : QObject()
{
	form = rf;
    parentItem = parent;
    itemData = data;
    haveProps = false;
    formInstantiated = false;
	currentTagSize=0.05; 	//initialize tag size to defult of SpinBox in Widget

}
//! [0]

//! [1]
TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}
//! [1]

//! [2]
TreeItem *TreeItem::child(int number)
{
    return childItems.value(number);
}
//! [2]

//! [3]
int TreeItem::childCount() const
{
    return childItems.count();
}
//! [3]

//! [4]
int TreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
//! [4]

//! [5]
int TreeItem::columnCount() const
{
    return itemData.count();
}
//! [5]

//! [6]
QVariant TreeItem::data(int column) const
{
    return itemData.value(column);
}
//! [6]

//! [7]
bool TreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        TreeItem *item = new TreeItem(data, form, this);
        childItems.insert(position, item);
    }

    return true;
}
//! [7]

//! [8]
bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (TreeItem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}
//! [8]

//! [9]
TreeItem *TreeItem::parent()
{
    return parentItem;
}
//! [9]

//! [10]
bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}
//! [10]

bool TreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (TreeItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}
void TreeItem::setViewer(BrainQuarter *quarterViewer) {
	viewer = quarterViewer;
}
//! [11]
bool TreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}
//! [11]


/*GeometryScene * TreeItem::insertGeometry(QFile &file, bool cylinder_flag,char* dbfile) {*/
// GeometryScene * TreeItem::insertGeometry(QFile &file) {
bool TreeItem::insertGeometry(QFile &file) {
	qDebug() << "Debug. >>TreeItem::insertGeometry()";
	QVector<QVariant> data(columnCount());
	GeometryScene *item = new GeometryScene(data, form, this);
	item->setViewer(viewer);
	childItems.insert(childCount(), item);
//     item->loadGeometry(file, data, form, this,cylinder_flag,dbfile);
    item->loadGeometry(file, data, form, this);

	qDebug() << "Debug. >>TreeItem::insertGeometry()";
//     return item;
	return true;
}

// GeometryScene * TreeItem::insertGeometry(QFile &file) {
// 	QVector<QVariant> data(columnCount());
// 	GeometryScene *item = new GeometryScene(data, form, this);
// 	item->setViewer(viewer);
// 	childItems.insert(childCount(), item);
// 	QFile nullfile;
//     item->loadGeometry(file, data, form, this,false,nullfile);
// 
//     return item;
// 
// }

bool TreeItem::insertTags(QFile &file) {
    QVector<QVariant> data(columnCount());
    tagFileItem *item = new tagFileItem(viewer->getRootSeparator(),
                                        data, form, this);
    childItems.insert(childCount(), item);
    item->loadFile(file);
}

bool TreeItem::createTag(float *tagpoint){
	QVector<QVariant> data(columnCount());
    tagFileItem *item = new tagFileItem(viewer->getRootSeparator(),
                                        data, form, this);
    childItems.insert(childCount(), item);
	std::cout<<"current tag size : " << currentTagSize << std::endl;
	item->showTag(tagpoint,currentTagSize);	
}

void TreeItem::updateTagSize(double newsize){
//	qDebug() << "Debug. >>TreeItem::updateTagSize()";
	QVector<QVariant> data(columnCount());
    tagFileItem *item = new tagFileItem(viewer->getRootSeparator(),
                                        data, form, this);
//	std::cout << "viewer root number of children: " << 	viewer->getRootSeparator()->getNumChildren() << std::endl;								
//     childItems.insert(childCount(), item);
// 	item->updateSize(newsize,currentTagSize);
	currentTagSize= newsize;		//update the tagsize for next tags
//	qDebug() << "Debug. <<TreeItem::updateTagSize()";
}

bool TreeItem::insertCone() {
	SoSeparator *root = viewer->getRootSeparator();
	SoBaseColor *col = new SoBaseColor;
	col->rgb = SbColor(1, 1, 0);
	root->addChild(col);
	root->addChild(new SoCone);
	viewer->viewAll();

	insertChildren(childCount(), 1, columnCount());
	child(childCount() - 1)->setData(0, "Cone");
}

QWidget* TreeItem::createForm() {
	qDebug() << "Debug. >>TreeItem::createForm()";

}

void TreeItem::destroyForm() {

}
bool TreeItem::createRootSeparator() {

}
