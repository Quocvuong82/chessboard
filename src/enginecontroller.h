#ifndef ENGINECONTROLLER_H
#define ENGINECONTROLLER_H

#include <QWidget>
#include "uciengine.h"
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
public slots:
    void go();
    void toggleGoStop();
    void showBestmove();
    void showOtherMoves();
    void showDepth(int);
    void updateController(int state);
    void undock();
    void play();
    void turnOff();
    void turnOn();
signals:
    void madeMove();
    void newBestmove(string);
private:
    Ui::EngineController *ui;
    UCIEngine* engine;
    Board* board;
    Game* game;
    bool goButtonPressed;
};

#endif // ENGINECONTROLLER_H
