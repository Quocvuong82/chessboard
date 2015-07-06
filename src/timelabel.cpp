#include "timelabel.h"

TimeLabel::TimeLabel()
{

}
void TimeLabel::setTime(int seconds) {
    time = seconds;
    this->QLabel::setText("<font size=20 color=" + color + "><b>" + makeTime(seconds) + "</b></font>");
}

QString TimeLabel::makeTime(int seconds) {
    int m;
    string timestr;
    m = seconds / 60;
    seconds = seconds - m * 60;
    if(m < 10) timestr.append("0");
    timestr.append(boost::lexical_cast<string>(m));
    timestr.append(":");
    if(seconds < 10) timestr.append("0");
    timestr.append(boost::lexical_cast<string>(seconds));
    return QString::fromStdString(timestr);
}

void TimeLabel::setColor(string color) {
    this->color = QString::fromStdString(color);
    this->QLabel::setText("<font size=20 color=" + this->color + "><b>" + makeTime( time ) + "</b></font>");
}
