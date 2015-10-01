#ifndef TIMELABEL_H
#define TIMELABEL_H
#include <QLabel>
#include <QLabel>
#include <string>
#include "boost/lexical_cast.hpp"

using namespace std;
class TimeLabel : public QLabel
{
Q_OBJECT
public:
    TimeLabel();
    TimeLabel(string color);
    void setTime(int seconds);
    void setColor(string color);
    bool isEnabled(); // Returns true if the clock is switched on
    void activate(bool enable);
protected:
  void mousePressEvent(QMouseEvent *event);
private:
    QString color;
    int time = 0; // time in seconds
    bool enabled;
    QString makeTime(int seconds);
signals:
    void clicked();
};



#endif // TIMELABEL_H
