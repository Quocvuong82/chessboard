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
    void setColor(string color);
private:
    QString color;
    QString name;
};

#endif // PLAYERLABEL_H
