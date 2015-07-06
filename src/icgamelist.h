#ifndef ICGAMELIST_H
#define ICGAMELIST_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class ICGameList;
}

class ICGameList : public QWidget
{
    Q_OBJECT

public:
    explicit ICGameList(QWidget *parent = 0);
    ~ICGameList();
    void addItem(QListWidgetItem *item);
    QListWidget* list;
private:
    Ui::ICGameList *ui;
signals:
    void itemClicked(QListWidgetItem *item);
public slots:
    void emitItemClicked(QListWidgetItem *item);
};

#endif // ICGAMELIST_H
