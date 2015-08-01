SOURCES += \
    chessboard.cpp \
    board.cpp \
    fen.cpp \
    move.cpp \
    mywindow.cpp \
    database.cpp \
    chessdatabase.cpp \
    enginethread.cpp \
    icclient.cpp \
    dbdialog.cpp \
    game.cpp \
    qsquare.cpp \
    icgamelist.cpp \
    playerlabel.cpp \
    timelabel.cpp
QT += core gui widgets
QT += svg
QT += webkitwidgets

HEADERS += \
    board.h \
    fen.h \
    move.h \
    mywindow.h \
    database.h \
    chessdatabase.h \
    enginethread.h \
    icclient.h \
    dbdialog.h \
    game.h \
    qsquare.h \
    icgamelist.h \
    playerlabel.h \
    timelabel.h
LIBS += -L/usr/lib/mysql -lmysqlclient
LIBS += -L/usr/lib/mysql -lboost_regex
LIBS += -L/usr/lib/mysql -lboost_thread
LIBS += -L/usr/lib/mysql -lboost_system
INCLUDEPATH += /usr/include/mysql
CONFIG   += c++11

FORMS += \
    icgamelist.ui
