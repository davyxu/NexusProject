/****************************************************************************
** Meta object code from reading C++ file 'QPropertyGrid.h'
**
** Created: Wed Oct 12 19:40:56 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EditorPCH.h"
#include "..\..\QPropertyGrid.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QPropertyGrid.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QPropertyGrid[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   15,   14,   14, 0x0a,
      61,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QPropertyGrid[] = {
    "QPropertyGrid\0\0Prop,Value\0"
    "valueChanged(QtProperty*,QVariant)\0"
    "OnTestButtonClicked()\0"
};

const QMetaObject QPropertyGrid::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QPropertyGrid,
      qt_meta_data_QPropertyGrid, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QPropertyGrid::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QPropertyGrid::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QPropertyGrid::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPropertyGrid))
        return static_cast<void*>(const_cast< QPropertyGrid*>(this));
    return QWidget::qt_metacast(_clname);
}

int QPropertyGrid::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 1: OnTestButtonClicked(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
