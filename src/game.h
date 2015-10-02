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
    ~Game();
    QBoard* board;
    static size_t getNrOfGames();
    int getGameID();
    char getActiveColor();
    int getCurrentMoveNr();
    void setActiveColor(char c);
    void move(string movecmd);
    void nextPos();
    void nextPos(int index);
    void prevPos();
    QTreeWidget* movehistory;
    void saveGame();
    void setTimeW(int time);
    void setTimeB(int time);
private:
    static size_t NrOfGames;
    int gameID;
    char activeColor;
    int currmoveNr = 0;
public slots:
    void showMoveHistory();
    void HistoryItemClicked(QTreeWidgetItem*, int);
    void setGameID(int id);
    void show();
    void hide();
};
#endif // GAME_H
