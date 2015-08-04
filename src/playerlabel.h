#ifndef PLAYERLABEL_H
#define PLAYERLABEL_H
#include <QLabel>
#include <string>
using namespace std;
class PlayerLabel : public QLabel
{
Q_OBJECT
public:
    PlayerLabel();
    PlayerLabel(QString text);
    void setText(string name);
    void setID(int ID);
    void setText(QString name);
    void setColor(string color);
protected:
  void mousePressEvent(QMouseEvent *event);
private:
    QString color;
    QString name;
    int ID;
public slots:
    void edit();
signals:
    void clicked(int);
};

#endif // PLAYERLABEL_H
