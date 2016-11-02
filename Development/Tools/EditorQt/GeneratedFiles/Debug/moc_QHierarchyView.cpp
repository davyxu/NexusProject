/****************************************************************************
** Meta object code from reading C++ file 'QHierarchyView.h'
**
** Created: Tue Oct 4 18:26:24 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EditorPCH.h"
#include "..\..\QHierarchyView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QHierarchyView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QHierarchyView[] = {

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
      16,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QHierarchyView[] = {
    "QHierarchyView\0\0OnItemSelectionChanged()\0"
};

const QMetaObject QHierarchyView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QHierarchyView,
      qt_meta_data_QHierarchyView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QHierarchyView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QHierarchyView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QHierarchyView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QHierarchyView))
        return static_cast<void*>(const_cast< QHierarchyView*>(this));
    if (!strcmp(_clname, "ECObjectIterateListener"))
        return static_cast< ECObjectIterateListener*>(const_cast< QHierarchyView*>(this));
    return QWidget::qt_metacast(_clname);
}

int QHierarchyView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnItemSelectionChanged(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
