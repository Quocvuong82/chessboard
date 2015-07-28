#ifndef QSQUARE_H
#define QSQUARE_H
#include <QLabel>

class QSquare : public QLabel {
    Q_OBJECT
public:
    QSquare();
    void emitSignal();
    void setPixmap(QPixmap p);
private:
    static int ID;
    int id;
    QPoint dragStartPosition;
    QLabel* piece;
    QPixmap square_d = QPixmap("./textures/wood_d.png");
    QPixmap square_l = QPixmap("./textures/wood_l.png");
protected:
  void mousePressEvent(QMouseEvent * e);
  void mouseMoveEvent(QMouseEvent *event);
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
signals:
    void clicked (int);
    void dropped (int, int);
};

#endif // QSQUARE_H
