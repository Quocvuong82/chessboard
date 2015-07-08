#include "dbdialog.h"

DBdialog::DBdialog(ChessDatabase chessDB){
    myChessDB = chessDB;
    //setFixedSize(150,100);
    QVBoxLayout *vbox = new QVBoxLayout;
    QPushButton *button1 = new QPushButton("OK");
    QPushButton *button2 = new QPushButton("Abbrechen");
    vbox->addWidget(button1);
    vbox->addWidget(button2);
    //setLayout(vbox);
    connect(button1, SIGNAL(clicked()), this, SLOT(mySetResult()));
    connect(button2, SIGNAL(clicked()), this, SLOT(reject()));

    eventID = 0;
    eventIDs = myChessDB.getEventIDs();

    selectionLayout = new QHBoxLayout;

    combo1 = new QComboBox;
    combo1->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    for(int i = 0; i < eventIDs.size(); i++) {
        combo1->addItem(QString::fromStdString(myChessDB.getEventByID(eventIDs[i])));
    }

    comboGames = new QComboBox;
    comboGames->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    results = new QListWidget();

    selectionLayout->addWidget(combo1);
    //selectionLayout->addWidget(comboGames);
    SelectionBox = new QGroupBox("Select a Game");
    SelectionBox->setLayout(selectionLayout);
    ButtonLayout = new QHBoxLayout;
    ButtonLayout->addWidget(button2);
    ButtonLayout->addWidget(button1);
    buttons = new QFrame();
    buttons->setLayout(ButtonLayout);
    layout = new QVBoxLayout;
    layout->addWidget(SelectionBox);

    input = new QLineEdit;

    /* Signal-Slot Connections */
    QObject::connect(combo1, SIGNAL(activated(int)), this, SLOT(setEvent(int)));
    //QObject::connect(comboGames, SIGNAL(activated(int)), this, SLOT(setGame(int)));
    QObject::connect(results, SIGNAL(currentRowChanged(int)), this, SLOT(setGame(int)));
    QObject::connect(input, SIGNAL(textChanged(QString)), this, SLOT(getGame(QString)));

    layout->addWidget(input);
    layout->addWidget(results);
    layout->addWidget(buttons);

    setLayout(layout);
    setEvent(0);
}

void DBdialog::setEvent(int value) {
    item.clear();
    results->clear();
    comboGames->clear();
    gameIDs.clear();
    eventID = eventIDs[value];
    vector<vector<int>> ids = myChessDB.getGameIDsByEventID(eventIDs[value]);
    string games;
    for(int i = 0; i < ids.size(); i++) { 
        gameIDs.push_back(ids[i][0]);
        games = myChessDB.getPlayerByID(ids[i][1]) + "-" + myChessDB.getPlayerByID(ids[i][2]);
        //comboGames->addItem(QString::fromStdString(games));
        item.push_back(new QListWidgetItem);
        item[i]->setText(QString::fromStdString(games));
        results->addItem(item[i]);
    }
    //layout->addWidget(results);
}

void DBdialog::getGame(QString txt) {
    item.clear();
    results->clear();
    comboGames->clear();
    gameIDs.clear();
    cout << "set event " << txt.toStdString() << endl;
    vector<vector<int>> ids = myChessDB.getGameIDsByString(txt.toStdString(), eventID);
    cout << "got ids " << endl;
    cout << ids.size() << endl;
    string games;
    for(int i = 0; i < ids.size(); i++) {
        gameIDs.push_back(ids[i][0]);
        games = myChessDB.getPlayerByID(ids[i][1]) + "-" + myChessDB.getPlayerByID(ids[i][2]);
        //comboGames->addItem(QString::fromStdString(games));
        item.push_back(new QListWidgetItem);
        item[i]->setText(QString::fromStdString(games));
        results->addItem(item[i]);
    }
    //layout->addWidget(results);
}

void DBdialog::setGame(int value) {
    //localboard = true;
    //posIndex = 0;
    //posIDs = myChessDB.getPosIDsByGameID(gameIDs[value]);
    cout << "game " << value << " set" << endl;
    gameID = gameIDs[value];
    //posID = posIDs[0];
    //updateBoard();
}

void DBdialog::mySetResult() {
    emit done(gameID);
}
