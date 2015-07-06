#include "icgame.h"
#include "ui_icgame.h"

ICGameList::ICGameList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::icgame)
{
    ui->setupUi(this);
}

ICGameList::~ICGameList()
{
    delete ui;
}
