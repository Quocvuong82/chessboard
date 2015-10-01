#ifndef QCHESSBASE_H
#define QCHESSBASE_H

#include <QObject>
#include <QWidget>
#include "chessdatabase.h"
#include "dbdialog.h"

/*namespace Ui {
class SetupDB;
}*/

class QChessBase : public QWidget, public ChessDatabase
{
    Q_OBJECT
public:
    QChessBase(string host, string user, string password, string database);
    explicit QChessBase(QWidget *parent = 0);
    DBdialog* dialog;
private:
    //Ui::SetupDB *ui;
    QDialog* setupDBDialog;
public slots:
    void setHost(QString host);
    void setUsername(QString name);
    void setPassword(QString password);
    void setDatabase(QString database);
    void showGameSelectDialog();
    void createNewDB();
    void setupDB();
    void handleGameSelection(int ID);
signals:
    void GameSelected(int ID);
};

#endif // QCHESSBASE_H
