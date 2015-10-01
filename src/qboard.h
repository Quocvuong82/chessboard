#ifndef QBOARD_H
#define QBOARD_H

#include <QWidget>
#include <QSvgWidget>
#include <QGridLayout>
#include "qsquare.h"
#include <QLabel>
#include <QGesture>
#include <QGestureEvent>
#include <QSwipeGesture>
#include <QMessageBox>
#include <QSlider>
#include <QTimer>
#include "fen.h"
#include "board.h"
#include "timelabel.h"
#include "playerlabel.h"

class QBoard : public QWidget, public Board
{
private:
    Q_OBJECT
    QPixmap* pieces[12];
    QSvgWidget* piecesSVG[12];
    QVector<QPainter*> painter;
    QPixmap square_d;
    QPixmap square_l;
    QVector<QSquare*> squares;
    QTimer* timer;
    int timeW = 1800; // Time White in Seconds
    int timeB = 1800; // Time Black in Seconds
    void initSquares();
    void writePositionTosquares();
    void highlightSquares();

    /* Style */
    QString StyleSheet;
protected:
      //bool event(QEvent* e);
      //bool gestureEvent(QGestureEvent* e);
      //void swipeTriggered(QSwipeGesture *gesture);
public:
    explicit QBoard(QWidget *parent = 0);
    ~QBoard();
    QGridLayout* Grid;
    QSlider* Slider;
    PlayerLabel* playerW;
    PlayerLabel* playerB;
    TimeLabel* timeLabelW;
    TimeLabel* timeLabelB;
    QLabel* playerPhotoW;
    QLabel* playerPhotoB;
    static QString PATH;
    void nextPos();
    void nextPos(int index);
    void prevPos();
    void loadFile(string filename);
    bool setPosition(Fen pos);
    void show();
    void move(string movecmd);
    void setTimeW(int time);
    void setTimeB(int time);
signals:
    void madeMove();
    void madeMove(string move);
public slots:
    void setPosition(int id);
    void squareClicked(int);
    void squareDropped(int, int);
    void hint(string); // Can be used to visualize move hints
    void playMoveSound();
    void setPlayerName(int playerID);
    void setPlayerWName();
    void setPlayerBName();
    void updateStyle(); // Apply style changes
    void updateTime();
};

#endif // QBOARD_H
