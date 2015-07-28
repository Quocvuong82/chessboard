#ifndef QSQUARE_H
#define QSQUARE_H
#include <QLabel>

class QSquare : public QLabel {
    Q_OBJECT
public:
    QSquare();
    void emitSignal();
private:
    static int ID;
    int id;
    QPoint dragStartPosition;
protected:
  void mousePressEvent(QMouseEvent * e);
  void mouseMoveEvent(QMouseEvent *event);
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
signals:
    void clicked (int);
};

#endif // QSQUARE_H
