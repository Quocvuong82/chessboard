/****************************************************************************
** Meta object code from reading C++ file 'mywindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chessboard/master/src/mywindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mywindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MyWindow_t {
    QByteArrayData data[34];
    char stringdata[417];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyWindow_t qt_meta_stringdata_MyWindow = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MyWindow"
QT_MOC_LITERAL(1, 9, 7), // "nextPos"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 7), // "prevPos"
QT_MOC_LITERAL(4, 26, 9), // "readInput"
QT_MOC_LITERAL(5, 36, 5), // "think"
QT_MOC_LITERAL(6, 42, 12), // "readICServer"
QT_MOC_LITERAL(7, 55, 14), // "parseICSOutput"
QT_MOC_LITERAL(8, 70, 6), // "string"
QT_MOC_LITERAL(9, 77, 10), // "ICSconnect"
QT_MOC_LITERAL(10, 88, 11), // "ICSgameList"
QT_MOC_LITERAL(11, 100, 23), // "onICGameListItemclicked"
QT_MOC_LITERAL(12, 124, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(13, 141, 17), // "sendInputToServer"
QT_MOC_LITERAL(14, 159, 12), // "sendToServer"
QT_MOC_LITERAL(15, 172, 16), // "checkInputDialog"
QT_MOC_LITERAL(16, 189, 6), // "clocks"
QT_MOC_LITERAL(17, 196, 14), // "setBoardActive"
QT_MOC_LITERAL(18, 211, 7), // "newGame"
QT_MOC_LITERAL(19, 219, 13), // "SquareClicked"
QT_MOC_LITERAL(20, 233, 13), // "SquareDropped"
QT_MOC_LITERAL(21, 247, 15), // "EnginePlayBlack"
QT_MOC_LITERAL(22, 263, 15), // "EnginePlayWhite"
QT_MOC_LITERAL(23, 279, 4), // "quit"
QT_MOC_LITERAL(24, 284, 9), // "setGameID"
QT_MOC_LITERAL(25, 294, 6), // "undock"
QT_MOC_LITERAL(26, 301, 7), // "scanICS"
QT_MOC_LITERAL(27, 309, 16), // "showPositionTree"
QT_MOC_LITERAL(28, 326, 12), // "itemExpanded"
QT_MOC_LITERAL(29, 339, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(30, 356, 12), // "showPosition"
QT_MOC_LITERAL(31, 369, 11), // "linkClicked"
QT_MOC_LITERAL(32, 381, 15), // "updateStatusBar"
QT_MOC_LITERAL(33, 397, 19) // "showBestmoveMessage"

    },
    "MyWindow\0nextPos\0\0prevPos\0readInput\0"
    "think\0readICServer\0parseICSOutput\0"
    "string\0ICSconnect\0ICSgameList\0"
    "onICGameListItemclicked\0QListWidgetItem*\0"
    "sendInputToServer\0sendToServer\0"
    "checkInputDialog\0clocks\0setBoardActive\0"
    "newGame\0SquareClicked\0SquareDropped\0"
    "EnginePlayBlack\0EnginePlayWhite\0quit\0"
    "setGameID\0undock\0scanICS\0showPositionTree\0"
    "itemExpanded\0QTreeWidgetItem*\0"
    "showPosition\0linkClicked\0updateStatusBar\0"
    "showBestmoveMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      30,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  164,    2, 0x0a /* Public */,
       3,    0,  165,    2, 0x0a /* Public */,
       4,    0,  166,    2, 0x0a /* Public */,
       5,    0,  167,    2, 0x0a /* Public */,
       6,    0,  168,    2, 0x0a /* Public */,
       7,    1,  169,    2, 0x0a /* Public */,
       9,    0,  172,    2, 0x0a /* Public */,
      10,    0,  173,    2, 0x0a /* Public */,
      11,    1,  174,    2, 0x0a /* Public */,
      13,    0,  177,    2, 0x0a /* Public */,
      14,    1,  178,    2, 0x0a /* Public */,
      15,    0,  181,    2, 0x0a /* Public */,
      15,    1,  182,    2, 0x0a /* Public */,
      16,    0,  185,    2, 0x0a /* Public */,
      17,    1,  186,    2, 0x0a /* Public */,
      18,    0,  189,    2, 0x0a /* Public */,
      19,    1,  190,    2, 0x0a /* Public */,
      20,    2,  193,    2, 0x0a /* Public */,
      21,    0,  198,    2, 0x0a /* Public */,
      22,    0,  199,    2, 0x0a /* Public */,
      23,    0,  200,    2, 0x0a /* Public */,
      24,    0,  201,    2, 0x0a /* Public */,
      25,    0,  202,    2, 0x0a /* Public */,
      26,    0,  203,    2, 0x0a /* Public */,
      27,    0,  204,    2, 0x0a /* Public */,
      28,    1,  205,    2, 0x0a /* Public */,
      30,    1,  208,    2, 0x0a /* Public */,
      31,    1,  211,    2, 0x0a /* Public */,
      32,    0,  214,    2, 0x0a /* Public */,
      33,    0,  215,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 29,    2,
    QMetaType::Void, 0x80000000 | 29,    2,
    QMetaType::Void, QMetaType::QUrl,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MyWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyWindow *_t = static_cast<MyWindow *>(_o);
        switch (_id) {
        case 0: _t->nextPos(); break;
        case 1: _t->prevPos(); break;
        case 2: _t->readInput(); break;
        case 3: _t->think(); break;
        case 4: _t->readICServer(); break;
        case 5: _t->parseICSOutput((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 6: { bool _r = _t->ICSconnect();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: _t->ICSgameList(); break;
        case 8: _t->onICGameListItemclicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 9: _t->sendInputToServer(); break;
        case 10: _t->sendToServer((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 11: _t->checkInputDialog(); break;
        case 12: _t->checkInputDialog((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->clocks(); break;
        case 14: _t->setBoardActive((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->newGame(); break;
        case 16: _t->SquareClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->SquareDropped((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->EnginePlayBlack(); break;
        case 19: _t->EnginePlayWhite(); break;
        case 20: _t->quit(); break;
        case 21: _t->setGameID(); break;
        case 22: _t->undock(); break;
        case 23: _t->scanICS(); break;
        case 24: _t->showPositionTree(); break;
        case 25: _t->itemExpanded((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 26: _t->showPosition((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 27: _t->linkClicked((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 28: _t->updateStatusBar(); break;
        case 29: _t->showBestmoveMessage(); break;
        default: ;
        }
    }
}

const QMetaObject MyWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MyWindow.data,
      qt_meta_data_MyWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyWindow.stringdata))
        return static_cast<void*>(const_cast< MyWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MyWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 30)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 30;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
