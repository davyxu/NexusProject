/****************************************************************************
** Meta object code from reading C++ file 'QMaterialPanel.h'
**
** Created: Sat Oct 22 19:41:23 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EditorPCH.h"
#include "..\..\QMaterialPanel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QMaterialPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QMaterialPanel[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      40,   15,   15,   15, 0x08,
      67,   15,   15,   15, 0x08,
      83,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QMaterialPanel[] = {
    "QMaterialPanel\0\0OnActionBuildMaterial()\0"
    "OnActionUpdateToSelected()\0OnActionReset()\0"
    "OnActionLoadMaterial()\0"
};

const QMetaObject QMaterialPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QMaterialPanel,
      qt_meta_data_QMaterialPanel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QMaterialPanel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QMaterialPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QMaterialPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QMaterialPanel))
        return static_cast<void*>(const_cast< QMaterialPanel*>(this));
    return QWidget::qt_metacast(_clname);
}

int QMaterialPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnActionBuildMaterial(); break;
        case 1: OnActionUpdateToSelected(); break;
        case 2: OnActionReset(); break;
        case 3: OnActionLoadMaterial(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
