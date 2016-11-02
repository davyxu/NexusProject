/****************************************************************************
** Meta object code from reading C++ file 'QMaterialScene.h'
**
** Created: Sun Oct 30 00:09:35 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EditorPCH.h"
#include "..\..\QMaterialScene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QMaterialScene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QMaterialGraphicesView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_QMaterialGraphicesView[] = {
    "QMaterialGraphicesView\0"
};

const QMetaObject QMaterialGraphicesView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_QMaterialGraphicesView,
      qt_meta_data_QMaterialGraphicesView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QMaterialGraphicesView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QMaterialGraphicesView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QMaterialGraphicesView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QMaterialGraphicesView))
        return static_cast<void*>(const_cast< QMaterialGraphicesView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int QMaterialGraphicesView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_QMaterialScene[] = {

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
      16,   15,   15,   15, 0x08,
      37,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QMaterialScene[] = {
    "QMaterialScene\0\0OnActionDisconnect()\0"
    "OnActionDeleteNode()\0"
};

const QMetaObject QMaterialScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_QMaterialScene,
      qt_meta_data_QMaterialScene, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QMaterialScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QMaterialScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QMaterialScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QMaterialScene))
        return static_cast<void*>(const_cast< QMaterialScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int QMaterialScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnActionDisconnect(); break;
        case 1: OnActionDeleteNode(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
