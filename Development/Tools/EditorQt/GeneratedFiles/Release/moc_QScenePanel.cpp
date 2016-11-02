/****************************************************************************
** Meta object code from reading C++ file 'QScenePanel.h'
**
** Created: Sun Oct 30 00:09:34 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EditorPCH.h"
#include "..\..\QScenePanel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QScenePanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QScenePanel[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      32,   12,   12,   12, 0x0a,
      58,   12,   12,   12, 0x0a,
      81,   12,   12,   12, 0x0a,
     103,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QScenePanel[] = {
    "QScenePanel\0\0OnActionFreeView()\0"
    "OnActionObjectTranslate()\0"
    "OnActionObjectRotate()\0OnActionObjectScale()\0"
    "OnActionAdjustSpace()\0"
};

const QMetaObject QScenePanel::staticMetaObject = {
    { &QRenderPanel::staticMetaObject, qt_meta_stringdata_QScenePanel,
      qt_meta_data_QScenePanel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QScenePanel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QScenePanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QScenePanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QScenePanel))
        return static_cast<void*>(const_cast< QScenePanel*>(this));
    return QRenderPanel::qt_metacast(_clname);
}

int QScenePanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QRenderPanel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnActionFreeView(); break;
        case 1: OnActionObjectTranslate(); break;
        case 2: OnActionObjectRotate(); break;
        case 3: OnActionObjectScale(); break;
        case 4: OnActionAdjustSpace(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
