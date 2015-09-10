SOURCES += \
    chessboard.cpp \
    board.cpp \
    fen.cpp \
    move.cpp \
    database.cpp \
    chessdatabase.cpp \
    icclient.cpp \
    dbdialog.cpp \
    game.cpp \
    qsquare.cpp \
    icgamelist.cpp \
    playerlabel.cpp \
    timelabel.cpp \
    player.cpp \
    mainwindow.cpp \
    enginecontroller.cpp \
    uciengine.cpp \
    qdialvalue.cpp \
    qboard.cpp \
    qchessbase.cpp
QT += core gui widgets
QT += svg
QT += webkitwidgets
QT += widgets multimedia

HEADERS += \
    board.h \
    fen.h \
    move.h \
    database.h \
    chessdatabase.h \
    icclient.h \
    dbdialog.h \
    game.h \
    qsquare.h \
    icgamelist.h \
    playerlabel.h \
    timelabel.h \
    player.h \
    mainwindow.h \
    enginecontroller.h \
    uciengine.h \
    qdialvalue.h \
    qboard.h \
    qchessbase.h
LIBS += -L/usr/lib/mysql -lmysqlclient
LIBS += -L/usr/lib/mysql -lboost_regex
LIBS += -L/usr/lib/mysql -lboost_thread
LIBS += -L/usr/lib/mysql -lboost_system
INCLUDEPATH += /usr/include/mysql
INCLUDEPATH += /usr/include
CONFIG   += c++11

FORMS += \
    icgamelist.ui \
    enginecontroller.ui \
    createdatabase.ui \
    setupdb.ui
