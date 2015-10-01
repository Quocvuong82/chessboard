#include "playerlabel.h"
#include <iostream>
#include <QInputDialog>
#include <QFile>

using namespace std;
PlayerLabel::PlayerLabel()
{
    color = "black";
    //connect( this, SIGNAL( clicked() ), this, SLOT( edit() ) );
    setStyleSheet("padding-right: 2px;");
}

void PlayerLabel::activate(bool enable) {

    QFile File("style/playerlabel.css");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    if(enable) setStyleSheet(StyleSheet);
    else setStyleSheet("padding-right: 2px;");
}

PlayerLabel::PlayerLabel(QString text) {
    this->QLabel::setText(text);
}

void PlayerLabel::setText(string name) {
    this->name = QString::fromStdString(name);
    this->QLabel::setText("<font size=20 color=" + color + "><b>" + this->name + "</b></font> ");
}

void PlayerLabel::setText(QString name) {
    this->name = name;
    this->QLabel::setText("<font size=20 color=" + color + "><b>" + this->name + "</b></font> ");
}

void PlayerLabel::setColor(string color) {
    this->color = QString::fromStdString(color);
    this->QLabel::setText("<font size=20 color=" + this->color + "><b>" + name + "</b></font> ");
}

QString PlayerLabel::getName() {
    return name;
}

void PlayerLabel::mousePressEvent(QMouseEvent *event) {
    std::cout << ID << " clicked" << endl;
    emit clicked(ID); emit clicked();
}

void PlayerLabel::edit() {
    bool ok;
    QString txt = QInputDialog::getText(this, "Enter a new name", "Enter a new name", QLineEdit::Normal, "Enter a name", &ok);
    if(ok && !txt.isEmpty()) setText(txt);
}

void PlayerLabel::setID(int ID) {
    this->ID = ID;
}
