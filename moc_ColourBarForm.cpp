/****************************************************************************
** Meta object code from reading C++ file 'ColourBarForm.h'
**
** Created: Wed Aug 20 14:14:44 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ColourBarForm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ColourBarForm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ColourBarForm[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_ColourBarForm[] = {
    "ColourBarForm\0"
};

const QMetaObject ColourBarForm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ColourBarForm,
      qt_meta_data_ColourBarForm, 0 }
};

const QMetaObject *ColourBarForm::metaObject() const
{
    return &staticMetaObject;
}

void *ColourBarForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ColourBarForm))
	return static_cast<void*>(const_cast< ColourBarForm*>(this));
    return QWidget::qt_metacast(_clname);
}

int ColourBarForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
