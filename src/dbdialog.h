#ifndef DATABASEDIALOG_H
#define DATABASEDIALOG_H
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QFrame>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QVector>
#include <iostream>
#include "boost/lexical_cast.hpp"
#include "chessdatabase.h"

class DBdialog : public QDialog
{
Q_OBJECT
public:
    DBdialog();
private:
    ChessDatabase myChessDB = ChessDatabase("localhost", "username", "password", "mychessdb");

    vector<int> eventIDs;
    vector<int> gameIDs;
    vector<int> posIDs;
    int gameID;
    int eventID; // selected event's id

    QHBoxLayout* selectionLayout;
    QVBoxLayout* layout;
    QHBoxLayout* ButtonLayout;
    QComboBox* combo1;
    QComboBox* comboGames;
    QGroupBox* SelectionBox;
    QFrame* buttons;
    QLineEdit* input;
    QListWidget* results;
    vector<QListWidgetItem*> item;

public slots:
    void setEvent(int value);
    void getGame(QString txt);
    void setGame(int value);
    void mySetResult();
};

#endif // DATABASEDIALOG_H
