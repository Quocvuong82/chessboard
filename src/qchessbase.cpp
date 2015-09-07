#include "qchessbase.h"
#include "ui_setupdb.h"
#include "ui_createdatabase.h"

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

    /* Database Default Values */
    host = "localhost";
    user = "root";
    password = "";
    database = "myChessDB";

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
    //dialog = new DBdialog(*db);
    dialog->setDatabase(*db); // workaround for signal-slot connection problem with board in mainwindow
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
