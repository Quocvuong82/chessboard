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
protected:
  void mousePressEvent(QMouseEvent * e);
signals:
    void clicked (int);
};

#endif // QSQUARE_H
