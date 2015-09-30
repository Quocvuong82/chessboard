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
#include "fen.h"
#include "board.h"

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
    void initSquares();
    void writePositionTosquares();
    void highlightSquares();
protected:
      //bool event(QEvent* e);
      //bool gestureEvent(QGestureEvent* e);
      //void swipeTriggered(QSwipeGesture *gesture);
public:
    explicit QBoard(QWidget *parent = 0);
    ~QBoard();
    QGridLayout* Grid;
    QSlider* Slider;
    static QString PATH;
    void nextPos();
    void nextPos(int index);
    void prevPos();
    void loadFile(string filename);
    bool setPosition(Fen pos);
    void show();
    void move(string movecmd);
signals:
    void madeMove();
public slots:
    void setPosition(int id);
    void squareClicked(int);
    void squareDropped(int, int);
    void hint(string); // Can be used to visualize move hints
    void playMoveSound();
};

#endif // QBOARD_H
