SOURCES += \
    chessboard.cpp \
    board.cpp \
    fen.cpp \
    move.cpp \
    database.cpp \
    chessdatabase.cpp \
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
INCLUDEPATH += "C:/Program Files (x86)/MySQL/MySQL Connector C 6.1/include"
INCLUDEPATH += "C:/Program Files/Boost/boost_1_59_0"
LIBS += "-LC:/Program Files (x86)/MySQL/MySQL Connector C 6.1/lib" -lmysql
LIBS += "C:/Windows/System32/ws2_32.dll"
LIBS += "-LC:/Program Files/Boost/boost_1_59_0/stage/lib/" -lboost_thread-mgw49-mt-d-1_59
LIBS += "-LC:/Program Files/Boost/boost_1_59_0/stage/lib/" -lboost_system-mgw49-mt-d-1_59
LIBS += "-LC:/Program Files/Boost/boost_1_59_0/stage/lib/" -lboost_regex-mgw49-mt-d-1_59
LIBS += "-LC:/Program Files (x86)/Tools/mingw492_32/lib/gcc/i686-w64-mingw32/4.9.2" -lstdc++
#LIBS += "-LC:/Program Files (x86)/5.5/mingw492_32/bin" -l
CONFIG   += c++11
CONFIG += static
QMAKE_CXXFLAGS += -march=i686

FORMS += \
    icgamelist.ui \
    enginecontroller.ui \
    createdatabase.ui \
    setupdb.ui
