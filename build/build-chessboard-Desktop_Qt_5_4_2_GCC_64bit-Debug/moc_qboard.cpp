/****************************************************************************
** Meta object code from reading C++ file 'qboard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chessboard/master/src/qboard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QBoard_t {
    QByteArrayData data[7];
    char stringdata[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QBoard_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QBoard_t qt_meta_stringdata_QBoard = {
    {
QT_MOC_LITERAL(0, 0, 6), // "QBoard"
QT_MOC_LITERAL(1, 7, 8), // "madeMove"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 11), // "setPosition"
QT_MOC_LITERAL(4, 29, 2), // "id"
QT_MOC_LITERAL(5, 32, 13), // "squareClicked"
QT_MOC_LITERAL(6, 46, 13) // "squareDropped"

    },
    "QBoard\0madeMove\0\0setPosition\0id\0"
    "squareClicked\0squareDropped"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QBoard[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   35,    2, 0x0a /* Public */,
       5,    1,   38,    2, 0x0a /* Public */,
       6,    2,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,

       0        // eod
};

void QBoard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QBoard *_t = static_cast<QBoard *>(_o);
        switch (_id) {
        case 0: _t->madeMove(); break;
        case 1: _t->setPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->squareClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->squareDropped((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QBoard::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QBoard::madeMove)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject QBoard::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QBoard.data,
      qt_meta_data_QBoard,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QBoard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QBoard::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QBoard.stringdata))
        return static_cast<void*>(const_cast< QBoard*>(this));
    if (!strcmp(_clname, "Board"))
        return static_cast< Board*>(const_cast< QBoard*>(this));
    return QWidget::qt_metacast(_clname);
}

int QBoard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void QBoard::madeMove()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
