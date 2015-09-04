/****************************************************************************
** Meta object code from reading C++ file 'uciengine.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chessboard/master/src/uciengine.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uciengine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UCIEngine_t {
    QByteArrayData data[16];
    char stringdata[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UCIEngine_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UCIEngine_t qt_meta_stringdata_UCIEngine = {
    {
QT_MOC_LITERAL(0, 0, 9), // "UCIEngine"
QT_MOC_LITERAL(1, 10, 9), // "newOutput"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 11), // "newBestmove"
QT_MOC_LITERAL(4, 33, 8), // "newDepth"
QT_MOC_LITERAL(5, 42, 12), // "stateChanged"
QT_MOC_LITERAL(6, 55, 13), // "printBestmove"
QT_MOC_LITERAL(7, 69, 10), // "showOutput"
QT_MOC_LITERAL(8, 80, 9), // "getValues"
QT_MOC_LITERAL(9, 90, 2), // "go"
QT_MOC_LITERAL(10, 93, 4), // "stop"
QT_MOC_LITERAL(11, 98, 14), // "setSearchDepth"
QT_MOC_LITERAL(12, 113, 5), // "value"
QT_MOC_LITERAL(13, 119, 11), // "setMovetime"
QT_MOC_LITERAL(14, 131, 7), // "setMate"
QT_MOC_LITERAL(15, 139, 8) // "setNodes"

    },
    "UCIEngine\0newOutput\0\0newBestmove\0"
    "newDepth\0stateChanged\0printBestmove\0"
    "showOutput\0getValues\0go\0stop\0"
    "setSearchDepth\0value\0setMovetime\0"
    "setMate\0setNodes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UCIEngine[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    0,   80,    2, 0x06 /* Public */,
       4,    1,   81,    2, 0x06 /* Public */,
       5,    1,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   87,    2, 0x0a /* Public */,
       7,    0,   88,    2, 0x0a /* Public */,
       8,    0,   89,    2, 0x0a /* Public */,
       9,    0,   90,    2, 0x0a /* Public */,
      10,    0,   91,    2, 0x0a /* Public */,
      11,    1,   92,    2, 0x0a /* Public */,
      13,    1,   95,    2, 0x0a /* Public */,
      14,    1,   98,    2, 0x0a /* Public */,
      15,    1,  101,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,

       0        // eod
};

void UCIEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UCIEngine *_t = static_cast<UCIEngine *>(_o);
        switch (_id) {
        case 0: _t->newOutput(); break;
        case 1: _t->newBestmove(); break;
        case 2: _t->newDepth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->printBestmove(); break;
        case 5: _t->showOutput(); break;
        case 6: _t->getValues(); break;
        case 7: _t->go(); break;
        case 8: _t->stop(); break;
        case 9: _t->setSearchDepth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->setMovetime((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->setMate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->setNodes((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (UCIEngine::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UCIEngine::newOutput)) {
                *result = 0;
            }
        }
        {
            typedef void (UCIEngine::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UCIEngine::newBestmove)) {
                *result = 1;
            }
        }
        {
            typedef void (UCIEngine::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UCIEngine::newDepth)) {
                *result = 2;
            }
        }
        {
            typedef void (UCIEngine::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UCIEngine::stateChanged)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject UCIEngine::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UCIEngine.data,
      qt_meta_data_UCIEngine,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UCIEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UCIEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UCIEngine.stringdata))
        return static_cast<void*>(const_cast< UCIEngine*>(this));
    return QObject::qt_metacast(_clname);
}

int UCIEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void UCIEngine::newOutput()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void UCIEngine::newBestmove()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void UCIEngine::newDepth(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void UCIEngine::stateChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_EngineReader_t {
    QByteArrayData data[7];
    char stringdata[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EngineReader_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EngineReader_t qt_meta_stringdata_EngineReader = {
    {
QT_MOC_LITERAL(0, 0, 12), // "EngineReader"
QT_MOC_LITERAL(1, 13, 8), // "finished"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 5), // "error"
QT_MOC_LITERAL(4, 29, 3), // "err"
QT_MOC_LITERAL(5, 33, 9), // "newOutput"
QT_MOC_LITERAL(6, 43, 7) // "process"

    },
    "EngineReader\0finished\0\0error\0err\0"
    "newOutput\0process"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EngineReader[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    1,   35,    2, 0x06 /* Public */,
       5,    0,   38,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void EngineReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EngineReader *_t = static_cast<EngineReader *>(_o);
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->newOutput(); break;
        case 3: _t->process(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (EngineReader::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EngineReader::finished)) {
                *result = 0;
            }
        }
        {
            typedef void (EngineReader::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EngineReader::error)) {
                *result = 1;
            }
        }
        {
            typedef void (EngineReader::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EngineReader::newOutput)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject EngineReader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_EngineReader.data,
      qt_meta_data_EngineReader,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *EngineReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EngineReader::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_EngineReader.stringdata))
        return static_cast<void*>(const_cast< EngineReader*>(this));
    return QObject::qt_metacast(_clname);
}

int EngineReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void EngineReader::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void EngineReader::error(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EngineReader::newOutput()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
