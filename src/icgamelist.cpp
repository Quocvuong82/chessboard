#include "icgamelist.h"
#include "ui_icgamelist.h"

ICGameList::ICGameList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICGameList)
{
    ui->setupUi(this);
    list = ui->listWidget;
}

ICGameList::~ICGameList()
{
    delete ui;
}

void ICGameList::addItem(QListWidgetItem *item) {
    ui->listWidget->addItem(item);
    //QObject::connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), MyWindow::, SLOT(onICGameListItemclicked(QListWidgetItem*)));
}

void ICGameList::emitItemClicked(QListWidgetItem *item) {
    emit itemClicked(item);
}
