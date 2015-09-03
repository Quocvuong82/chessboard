#ifndef GAME_H
#define GAME_H
#include "board.h"
#include "qboard.h"
#include <QTreeWidget>

class Game : public QObject
{
Q_OBJECT
public:
    Game();
    QBoard* board;
    static size_t getNrOfGames();
    int getGameID();
    void setGameID(int id);
    char getActiveColor();
    int getCurrentMoveNr();
    void setActiveColor(char c);
    void move(string movecmd);
    void nextPos();
    void nextPos(int index);
    void prevPos();
    QTreeWidget* movehistory;
    void saveGame();
private:
    static size_t NrOfGames;
    int gameID;
    char activeColor;
    int currmoveNr = 0;
public slots:
    void showMoveHistory();
    void HistoryItemClicked(QTreeWidgetItem*, int);
};
#endif // GAME_H
