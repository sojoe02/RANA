/****************************************************************************
** Meta object code from reading C++ file 'postcontrol.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/postprocessing/postcontrol.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'postcontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PostControl_t {
    QByteArrayData data[11];
    char stringdata0[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PostControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PostControl_t qt_meta_stringdata_PostControl = {
    {
QT_MOC_LITERAL(0, 0, 11), // "PostControl"
QT_MOC_LITERAL(1, 12, 20), // "startEventProcessing"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 9), // "eventPath"
QT_MOC_LITERAL(4, 44, 4), // "from"
QT_MOC_LITERAL(5, 49, 2), // "to"
QT_MOC_LITERAL(6, 52, 14), // "timeResolution"
QT_MOC_LITERAL(7, 67, 9), // "agentPath"
QT_MOC_LITERAL(8, 77, 13), // "mapResolution"
QT_MOC_LITERAL(9, 91, 11), // "zThresshold"
QT_MOC_LITERAL(10, 103, 14) // "on_processDone"

    },
    "PostControl\0startEventProcessing\0\0"
    "eventPath\0from\0to\0timeResolution\0"
    "agentPath\0mapResolution\0zThresshold\0"
    "on_processDone"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PostControl[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    7,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::QString, QMetaType::Int, QMetaType::Double,    3,    4,    5,    6,    7,    8,    9,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void PostControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PostControl *_t = static_cast<PostControl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startEventProcessing((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< double(*)>(_a[7]))); break;
        case 1: _t->on_processDone(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PostControl::*_t)(QString , int , int , double , QString , int , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PostControl::startEventProcessing)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject PostControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PostControl.data,
      qt_meta_data_PostControl,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PostControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PostControl::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PostControl.stringdata0))
        return static_cast<void*>(const_cast< PostControl*>(this));
    return QObject::qt_metacast(_clname);
}

int PostControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void PostControl::startEventProcessing(QString _t1, int _t2, int _t3, double _t4, QString _t5, int _t6, double _t7)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
