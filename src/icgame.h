#ifndef ICGAMELIST_H
#define ICGAMELIST_H

#include <QWidget>

namespace Ui {
class ICGameList;
}

class ICGameList : public QWidget
{
    Q_OBJECT

public:
    explicit ICGameList(QWidget *parent = 0);
    ~ICGameList();

private:
    Ui::ICGameList *ui;
};

#endif // ICGameList_H
