#ifndef TIMELABEL_H
#define TIMELABEL_H
#include <QLabel>
#include <QLabel>
#include <string>
#include "boost/lexical_cast.hpp"

using namespace std;
class TimeLabel : public QLabel
{
public:
    TimeLabel();
    void setTime(int seconds);
    void setColor(string color);
private:
    QString color;
    int time; // time in seconds
    QString makeTime(int seconds);
};



#endif // TIMELABEL_H
