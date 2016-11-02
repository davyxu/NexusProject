/****************************************************************************
** Meta object code from reading C++ file 'QProjectPanel.h'
**
** Created: Sat Oct 22 19:41:23 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EditorPCH.h"
#include "..\..\QProjectPanel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QProjectPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QProjectPanel[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      27,   15,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QProjectPanel[] = {
    "QProjectPanel\0\0item,column\0"
    "OnItemClicked(QTreeWidgetItem*,int)\0"
};

const QMetaObject QProjectPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QProjectPanel,
      qt_meta_data_QProjectPanel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QProjectPanel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QProjectPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QProjectPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QProjectPanel))
        return static_cast<void*>(const_cast< QProjectPanel*>(this));
    return QWidget::qt_metacast(_clname);
}

int QProjectPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnItemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
