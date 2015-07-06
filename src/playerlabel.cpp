#include "playerlabel.h"
using namespace std;
PlayerLabel::PlayerLabel()
{
    color = "black";
}
PlayerLabel::PlayerLabel(QString text) {
    this->QLabel::setText(text);
}

void PlayerLabel::setText(string name) {
    this->name = QString::fromStdString(name);
    this->QLabel::setText("<font size=20 color=" + color + "><b>" + this->name + "</b></font>");
}

void PlayerLabel::setColor(string color) {
    this->color = QString::fromStdString(color);
    this->QLabel::setText("<font size=20 color=" + this->color + "><b>" + name + "</b></font>");
}
