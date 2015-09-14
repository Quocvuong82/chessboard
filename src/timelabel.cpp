#include "timelabel.h"
#include <iostream>

TimeLabel::TimeLabel()
{
    enabled = true; // Make this timelabel available for the board's clock
}
void TimeLabel::setTime(int seconds) {
    time = seconds;
    this->QLabel::setText("<font size=20 color=" + color + ">" + makeTime(seconds) + "</font>");
}

bool TimeLabel::isEnabled() {
    return enabled;
}

void TimeLabel::activate(bool enable) {
    if(enable) setStyleSheet("background-color:#ebf3fc; border-top-right-radius: 9px; border-bottom-right-radius: 9px;");
    else setStyleSheet("");
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
    this->QLabel::setText("<font size=20 color=" + this->color + ">" + makeTime( time ) + "</font>");
}

void TimeLabel::mousePressEvent(QMouseEvent *event) {
    std::cout << "timelabel clicked" << endl;
    emit clicked();
    if(enabled) enabled = false; else enabled = true;
}
