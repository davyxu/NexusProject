/****************************************************************************
** Meta object code from reading C++ file 'QEditorFrame.h'
**
** Created: Sat Oct 22 19:41:23 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EditorPCH.h"
#include "..\..\QEditorFrame.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QEditorFrame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QEditorFrame[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      29,   13,   13,   13, 0x0a,
      44,   13,   13,   13, 0x0a,
      53,   13,   13,   13, 0x0a,
      66,   13,   13,   13, 0x0a,
      80,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QEditorFrame[] = {
    "QEditorFrame\0\0OnSaveLayout()\0"
    "OnLoadLayout()\0OnTick()\0OnNewScene()\0"
    "OnLoadScene()\0OnSaveScene()\0"
};

const QMetaObject QEditorFrame::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QEditorFrame,
      qt_meta_data_QEditorFrame, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QEditorFrame::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QEditorFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QEditorFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QEditorFrame))
        return static_cast<void*>(const_cast< QEditorFrame*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QEditorFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnSaveLayout(); break;
        case 1: OnLoadLayout(); break;
        case 2: OnTick(); break;
        case 3: OnNewScene(); break;
        case 4: OnLoadScene(); break;
        case 5: OnSaveScene(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
