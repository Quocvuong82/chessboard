#include "qchessbase.h"
#include "ui_setupdb.h"
#include "ui_createdatabase.h"
#include <QFile>
using namespace std;

QChessBase::QChessBase(string host, string user, string password, string database):
    QWidget(), ChessDatabase(host, user, password, database) {

}
QChessBase::QChessBase(QWidget *parent) :
    QWidget(parent),
    ChessDatabase() {

    /* Create a new Database-Setup-Dialog */
    setupDBDialog = new QDialog(0,0);
    Ui::SetupDB ui;
    ui.setupUi(setupDBDialog);

    /* Load Database Default Values from File */
    QFile File(".settings");
    File.open(QFile::ReadOnly);

    QString host = File.readLine().trimmed();
    this->host = host.toStdString(); //"localhost";
    QString user = File.readLine().trimmed();
    this->user = user.toStdString(); //"root";
    QString password = File.readLine().trimmed();
    this->password = password.toStdString(); //"";
    QString database = File.readLine().trimmed();
    this->database = database.toStdString(); //"myChessDB";

    ui.host->setText(host);
    ui.user->setText(user);
    ui.password->setText(password);
    ui.database->setText(database);

    /* Setup Database Connection */
    connect(ui.host, SIGNAL(textChanged(QString)), this, SLOT(setHost(QString)));
    connect(ui.user, SIGNAL(textChanged(QString)), this, SLOT(setUsername(QString)));
    connect(ui.password, SIGNAL(textChanged(QString)), this, SLOT(setPassword(QString)));
    connect(ui.database, SIGNAL(textChanged(QString)), this, SLOT(setDatabase(QString)));

    /* Create new Database with the parameters set */
    connect(ui.createDBButton, SIGNAL(clicked()), this, SLOT(createNewDB()));

    //connect(ui.connectButton, SIGNAL(clicked(), this, SLOT())

    /* Execute Setup-Dialog */
    setupDBDialog->setAttribute(Qt::WA_QuitOnClose);

    /* Create Game Select Dialog */
    ChessDatabase *db = this;
    dialog = new DBdialog(*db);
    QObject::connect(dialog, SIGNAL(finished(int)), this, SLOT(handleGameSelection(int)));
    //QObject::connect(dialog, SIGNAL(rejected()), this, SLOT(checkInputDialog()));
}

void QChessBase::setHost(QString host) {
    cout << "setting host to " << host.toStdString() << endl;
    this->host = host.toStdString();
}

void QChessBase::setUsername(QString name) {
    cout << "setting username to " << name.toStdString() << endl;
    this->user = name.toStdString();
}
void QChessBase::setPassword(QString password) {
    cout << "setting password to " << password.toStdString() << endl;
    this->password = password.toStdString();
}
void QChessBase::setDatabase(QString database) {
    cout << "setting database to " << database.toStdString() << endl;
    this->database = database.toStdString();
}

void QChessBase::showGameSelectDialog() {
    /* Create Game Select Dialog */
    ChessDatabase *db = this;
    dialog = new DBdialog(*db);
    QObject::connect(dialog, SIGNAL(finished(int)), this, SLOT(handleGameSelection(int)));
    dialog->exec();
}

void QChessBase::createNewDB() {
    QDialog* createDB = new QDialog();
    Ui::CreateDB ui;
    ui.setupUi(createDB);
    //createDB->exec();
    createChessDatabase();
}

void QChessBase::setupDB() {
    setupDBDialog->exec();
}

void QChessBase::handleGameSelection(int ID) {
    emit GameSelected(ID);
}
