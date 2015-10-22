#ifndef ENGINECONTROLLER_H
#define ENGINECONTROLLER_H

#include <QWidget>
#include <QListWidgetItem>
#include "uciengine.h"
#include "chessengine.h"
#include "board.h"
#include "game.h"

namespace Ui {
class EngineController;
}

class EngineController : public QWidget
{
    Q_OBJECT

public:
    explicit EngineController(QWidget *parent = 0);
    ~EngineController();
    void setBoard(Board* board);
    void setGame(Game* game);
    bool isOn();
    int getBestmoveScore();
private:
    UCIEngine* uci;
    ChessEngine* chess;
public slots:
    void go();
    void toggleGoStop();
    void showBestmove();
    void showOtherMoves();
    void showMove(string move);
    void showMove(QListWidgetItem* item);
    void showDepth(int);
    void updateController(int state);
    void selectEngine(int index);
    void undock();
    void play();
    void turnOff();
    void turnOn();
signals:
    void madeMove();
    void newBestmove(string);
    void newBestmoveScore(int);
private:
    Ui::EngineController *ui;
    Engine* engine;
    Board* board;
    Game* game;
    bool goButtonPressed;
    char activeColor;
};

#endif // ENGINECONTROLLER_H
