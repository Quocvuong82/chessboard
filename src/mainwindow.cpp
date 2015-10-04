#include "mainwindow.h"
#include <QtCore>
#include <QtGui>
#include "fen.h"
#include <QAction>
#include"uciengine.h"
#include "ui_playerwidget.h"

MainWindow::MainWindow(QMainWindow *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags) {

    /* Boolean Variables */
    chessserver = false;
    getICGameList = false;
    chessengine = false;
    localboard = true;
    engineB = false;
    engineW = false;
    moved = false;

    /* Init Modules */
    engineController = new EngineController();
    myChessDB = new QChessBase();

    /* Init Layout */
    GameInfoBox = new QGroupBox();
    next = new QPushButton("next");
    back = new QPushButton("back");
    nextCombo = new QComboBox;
    for(int i = 0; i < NrOfButtons; i++) {
        button.push_back(new QPushButton(QString("B").append(QString(QString::fromStdString(boost::lexical_cast<string>(i))))));
        button[i]->setMaximumWidth(50);
    };
    layout = new QHBoxLayout;
    ButtonBoxLayout = new QHBoxLayout;
    boardSliderBox = new QVBoxLayout();
    playersLayout = new QVBoxLayout;
    GameInfoLayout = new QVBoxLayout;
    QVBoxLayout* engineControllerLayout = new QVBoxLayout();

    BoardTab = new QTabWidget();

    /* Statusbar */
    statusActiveColor = new QLabel();
    statusMoveNr = new QLabel();

    /* Player Layout (Name, Time, Color, Score,...) */
    for(int i = 0; i < 2; i++) {
        playerFrame.push_back(new QFrame());
        playerLayout.push_back(new QHBoxLayout);
        score.push_back(new QLabel);
    }

    QGroupBox* engineControllerGroup = new QGroupBox();
    centralWidget = new QWidget;

    createMenu();  
    createToolBars();
    connectWidgets(); // Set up Signal-Slot-Connections

    /* Buttons */
    nextCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    nextCombo->hide();

    button[0]->setText("<");
    button[1]->setText(">");

    boardSliderBox->addWidget(BoardTab);

    statusBar();
    statusBar()->addPermanentWidget(statusMoveNr);
    statusBar()->addPermanentWidget(statusActiveColor);

    layout->addLayout(boardSliderBox);
    layout->addWidget(GameInfoBox);

    /* Create Games and Boards */
    for(int i = 0; i < NrOfBoards; i++) {
        newGame();
    }
    BoardTab->setTabsClosable(true);
    BoardTab->setCurrentIndex(0); // Show first Tab

    /* Buttons */
    for(int i = 0; i < NrOfButtons; i++) {
        ButtonBoxLayout->addWidget(button[i]);
    }
    ButtonBoxLayout->addWidget(nextCombo);

    /* Player Info */
    for(int i = 0; i < 2; i++) {
        playerLayout[i]->addWidget(score[i]);
        playerLayout[i]->setSpacing(0); // remove spacing between playerlabel and timelabel
        playerFrame[i]->setLayout(playerLayout[i]);  
    }

    /* Game Info */
    playersLayout->addWidget(playerFrame[0]);
    playersLayout->addWidget(playerFrame[1]);

    GameInfoLayout->addLayout(playersLayout);
    GameInfoLayout->addLayout(ButtonBoxLayout);

    engineControllerGroup->setLayout(engineControllerLayout);
    QString style = "QGroupBox {border: 1px solid gray;border-radius: 9px;margin-top: 0.5em;} QGroupBox::title {subcontrol-origin: margin;left: 10px;padding: 0 3px 0 3px;}";
    engineControllerGroup->setStyleSheet(style);
    engineControllerGroup->setTitle("Engine Controller");
    engineControllerLayout->addWidget(engineController);
    GameInfoLayout->addWidget(engineControllerGroup);
    GameInfoBox->setLayout(GameInfoLayout);

    /* Show Player Names, Time Labels, Player Photos, Slider and Movehistory */
    game[activeBoard]->show();

    /* Get position data from Database and display it on the GUI board */
    for(int i = 0; i < game.size(); i++) {
        game[i]->board->show(); // Write position to squares (QLabels)
        posIDs.push_back(vector<int> ());
        posIndex.push_back(0);
    }

    /* Set up EngineController */
    engineController->show();
    engineController->setGame(game[0]);

    layout->setMargin(0);
    centralWidget->setLayout(layout);

    this->setCentralWidget(centralWidget);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setWindowTitle("Chessboard 0.7.1");
    this->resize(1200, 400);
    this->adjustSize();
}  

void MainWindow::setBoardActive(int index) {
    if(index < 0) return;
    cout << "board " + boost::lexical_cast<string>(index) + " set active" << endl;

    /* Hide old Board and show the new one */
    game[activeBoard]->hide();
    game[index]->show();

    activeBoard = index; // Activeate new Board

    game[activeBoard]->board->show();
    engineController->setGame(game[index]);

    /* Hide Movehistory and Slider of inactive games */
    for(int i = 0; i < game.size(); i++) {
        game[i]->board->Slider->hide(); /* Game-Progress-Slider */
        game[i]->movehistory->hide();
    }
    game[activeBoard]->board->Slider->show();
    game[activeBoard]->movehistory->show();

    /* Update File Menu */
    fileMenu->removeAction(openFile);
    fileMenu->removeAction(saveToFile);
    openFile = new QAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Open File"), this);
    saveToFile = new QAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Save Game to File"), this);
    connect(openFile, SIGNAL(triggered()), game[activeBoard]->board, SLOT(openFile()));
    connect(saveToFile, SIGNAL(triggered()), game[activeBoard]->board, SLOT(saveGameToFile()));

    fileMenu->addAction(openFile);
    fileMenu->addAction(saveToFile);
    fileMenu->insertAction(quitAction, openFile);
    fileMenu->insertAction(quitAction, saveToFile);

    connect(game[activeBoard]->board, SIGNAL(madeMove()), this, SLOT(updateStatusBar()));
    updateStatusBar();
}

void MainWindow::checkInputDialog() {
    int val = myChessDB->dialog->result();
    if( val == QDialog::Accepted) {
        cout << "Akzeptiert!" << endl;
    } else if (val == QDialog::Rejected) {
        cout << "Abgelehnt!" << endl;
    }
}

void MainWindow::checkInputDialog(int gameID) {
    cout << "checkInputDialog " << gameID << endl;
    if (!myChessDB->dialog->result() == QDialog::Rejected) {
        localboard = true;
        game[activeBoard]->board->loadGame(gameID);
        cout << "game loaded" << endl;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
   switch(e->key()) {
   case Qt::Key_Right:
       nextPos();
       break;
   case Qt::Key_Left:
       prevPos();
       break;
   }
   updateStatusBar();
}

void MainWindow::nextPos() {
    if(chessserver && examining && !localboard) {
       fics.writeSocket("forward\n");
    } else {
        game[activeBoard]->nextPos(nextPosIndex);
        getNextPositions();
        //game[activeBoard]->board->show();
        /*if(posIDs[activeBoard].size() > 0 && (posIndex[activeBoard] + 1) < posIDs[activeBoard].size()) {
            posIndex[activeBoard]++;
            updateBoard();
        }*/
    }
}

void MainWindow::getNextPositions() {
    nextPosIndex = 0;
    if(game[activeBoard]->board->hasChildren()) {
        nextCombo->clear();
        vector<Fen*> nextPositions = game[activeBoard]->board->getChildren();
        if(nextPositions.size() > 1) {
            for(int i = 0; i < nextPositions.size(); i++) {
                nextCombo->addItem(QString::fromStdString(boost::lexical_cast<string>(myChessDB->getPositionIDFromDB(*nextPositions[i]))));
            }
            nextCombo->show();
        } else nextCombo->hide();
    }
}

void MainWindow::prevPos() {
    if(chessserver && examining && !localboard) {
       fics.writeSocket("backward\n");
    } else {
        unsetEngine(0);
        unsetEngine(1);
        game[activeBoard]->prevPos();
        getNextPositions();
        /*if(posIndex[activeBoard]>0) {
            posIndex[activeBoard]--;
            updateBoard();
        }*/
    }
}

/* Internet Chess Server Slots                                                */
/* -------------------------------------------------------------------------- */

bool MainWindow::ICSconnect() {
    chessserver = true;
    localboard = false;
    fics.connect();
    icgamelist = new ICGameList();
    QObject::connect(icgamelist->list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onICGameListItemclicked(QListWidgetItem*)));
    button[2]->disconnect();
    button[2]->setText("Get Game");
    connect(button[2], SIGNAL(clicked()), &fics, SLOT(getGame()));
    connect(&fics, SIGNAL(newLine(QString)), this, SLOT(parseICSOutput(QString)));
}

void MainWindow::sendToServer(string msg) {
    cout << "sending to server: '" << msg << "'" << endl;
    size_t o = msg.find("observe ");
    size_t ex = msg.find("examine ");
    //cout << "o: " << o << endl;
    if(o != string::npos) {
        size_t sp = msg.find(" ", o + 8);
        cout << "sp: " << sp << endl;
        cout << msg.substr(o + 8, sp - o - 8) << endl;
        int gameID = boost::lexical_cast<int>(msg.substr(o + 8, sp - o - 8));
        game[activeBoard]->setGameID(gameID);
        BoardTab->setTabText(activeBoard, QString::fromStdString("#" + boost::lexical_cast<string>(gameID)));
        cout << "set gameID to " << msg.substr(o + 8, sp - o - 8) << endl;
    }
    if(ex != string::npos) {
        size_t sp = msg.find(" ", ex + 8);
        size_t sp2 = msg.find(" ", sp + 1);
        sp++;
        cout << "sp: " << sp2 << endl;
        cout << msg.substr(sp, sp2 - sp) << endl;
        int gameID = boost::lexical_cast<int>(msg.substr(sp, sp2));
        game[activeBoard]->setGameID(gameID); // will not work, cause this is not the real game id (it's the ID from history)
        BoardTab->setTabText(activeBoard, QString::fromStdString("#" + boost::lexical_cast<string>(gameID)));
        cout << "set gameID to " << msg.substr(sp, sp2) << endl;
    }
    if(fics.isConnected()) fics.writeSocket(msg);
    //fics.writeSocket("help\n");
}

void MainWindow::setGameID() {
    bool ok;
    int gameID = QInputDialog::getInt(
                this, "Set Game ID", "Enter a new Game ID", 10, 0 , 999, 1, &ok);
    if(ok) game[activeBoard]->setGameID(gameID);

}

void MainWindow::setActiveColor() {
    bool ok;
    int activeColor = QInputDialog::getInt(
                this, "Set active color", "Enter a new color", 0, 0 , 1, 1, &ok);
    if(ok) {
        if(activeColor == 1) game[activeBoard]->setActiveColor('b');
        else game[activeBoard]->setActiveColor('w');
    }
}

void MainWindow::parseICSOutput(QString s) {
    /* Convert QString to String */
    string outstr  = s.toStdString();

    string position;
    vector<string> fenstrings(13);

    size_t g = outstr.find("games");
    //boost::regex expr("[0-9]+[ ][0-9+-]+[ ]");
    //boost::regex expr("[0-9]+[ ][0-9+-]+[ ][A-z]+[ ][0-9+-]+[ ][A-z]+");
    boost::regex expr("[0-9]+[ ][0-9+-]+[ ][A-z]+[ ]+[0-9+-]+[ ][A-z]+[ ]+[\[][ ]?");

    boost::smatch m;
    if(boost::regex_search(outstr, m, expr)) {
        getICGameList = true;
        item.push_back(new QListWidgetItem);
        item[item.size() - 1]->setText(QString::fromStdString(outstr.substr(0, outstr.size() - 1)));
        icgamelist->addItem(item[item.size() - 1]);
    } else if(getICGameList) {
        cout << "game list end" << endl;
        icgamelist->show(); getICGameList = false;
    }


    /* Parse style12-string */
    if(outstr.find("<12>") != string::npos) {
        cout << "style12 found " << endl;
        position.append(outstr.substr(outstr.find("<12>")));
        for (int j = 0; j < 8; j++) {
            fenstrings[j] = position.substr((5 + 9 * j), 8);
        }


        size_t b = 0; size_t e;
        int x = 0;
        e = position.find(" ", b + 1);
        values.clear();

        /* Get values from style12-string */
        while(e != string::npos && x < 50) {
            values.push_back(position.substr(b + 1,e - (b+1)));
            x++;
            b = e;
            e = position.find(" ", b + 1);
        }

        //cout << "set active color " << endl;
        fenstrings[8] = values[9][0] + 32; // active color (--> to lower case)
        //cout << "set castling values" << endl;
        if(values[11] == "1") fenstrings[9] = "K"; else fenstrings[9] = "-";
        if(values[12] == "1") fenstrings[9] += "Q"; else fenstrings[9] += "-";
        if(values[13] == "1") fenstrings[9] += "k"; else fenstrings[9] += "-";
        if(values[14] == "1") fenstrings[9] += "q"; else fenstrings[9] += "-";
        //fenstrings[9] += " ";
        if(values[10] == "-1") fenstrings[10] += "-"; else fenstrings[10] += values[10];
        //fenstrings[9] += " ";
        //cout << "set other value " << endl;
        fenstrings[11] += values[15];
        //fenstrings[9] += " ";
        //cout << "set move number" << endl;
        fenstrings[12] += values[26];
        //cout << fenstrings[9] << endl;

        /*for(int i=0; i < values.size(); i++) {
            cout << values[i] << "; ";
        }
        cout << endl;
        */
        bool isSet = false;
        for(int i = 0; i < game.size(); i++) {
            cout << game[i]->getGameID() << " " << values[16] << endl;
            if(game[i]->getGameID() == boost::lexical_cast<int>(values[16])) {
                cout << "set position" << endl;
                game[i]->board->setPosition(Fen(fenstrings));
                game[i]->setActiveColor(values[9][0] + 32); // Uppercase to lower case
                //game[i]->board->show();
                isSet = true;
                break;
            }
        }
        if(!isSet) {
            newGame();
            game[activeBoard]->setGameID(boost::lexical_cast<int>(values[16]));
            cout << "setting GameID to " << boost::lexical_cast<int>(values[16]) << endl;
            cout << "set position" << endl;
            game[activeBoard]->board->setPosition(Fen(fenstrings));
            game[activeBoard]->setActiveColor(values[9][0] + 32); // Uppercase to lower case
        }

        /* If the game id of the server message matches the game id of the active board... */
        /* Set time and player names */
        if(game[activeBoard]->getGameID() == boost::lexical_cast<int>(values[16])) {
            /* write data to playerinfo labels */
            game[activeBoard]->setTimeW(boost::lexical_cast<int>(values[24]));
            game[activeBoard]->setTimeB(boost::lexical_cast<int>(values[25]));
            game[activeBoard]->board->playerW->setText(values[17]);
            game[activeBoard]->board->playerB->setText(values[18]);
            game[activeBoard]->board->show();
        }
        updateStatusBar();

    }
}

void MainWindow::updateBoard() {
    //posIDLabel->setText(QString::number(posIDs[posIndex]));
    game[activeBoard]->board->getPositionFromDBByID(posIDs[activeBoard][posIndex[activeBoard]]);
    //fenstring->setText(QString::fromStdString(board->getFenstring()));
    game[activeBoard]->board->show();
}

void MainWindow::newGame() {
    cout << "new game " << endl;
    game.push_back(new Game());
    game[game.size() - 1]->board->getPositionFromDBByID(1);
    game[game.size() - 1]->board->show(); // Write position to squares (QLabels)
    game[game.size() - 1]->board->newGame();
    posIDs.push_back(vector<int> ());
    posIndex.push_back(0);
    BoardBox.push_back(new QFrame);
    //BoardBox[BoardBox.size() - 1]->setFixedSize(576,576);
    BoardTab->addTab(BoardBox[BoardBox.size() - 1], "Game " + QString::fromStdString(boost::lexical_cast<string>(BoardBox.size())));
    BoardBox[BoardBox.size() - 1]->setLayout(game[BoardBox.size() - 1]->board->Grid);
    BoardTab->setCurrentIndex(game.size() - 1);
    boardSliderBox->addWidget(game[game.size() - 1]->board->Slider);
    game[game.size() - 1]->board->Slider->show();
    game[game.size() - 1]->movehistory->show();

    playerLayout[1]->insertWidget(0, game[game.size() - 1]->board->playerPhotoW);
    playerLayout[1]->insertWidget(1, game[game.size() - 1]->board->playerW);
    playerLayout[1]->insertWidget(2, game[game.size() - 1]->board->timeLabelW);
    playerLayout[0]->insertWidget(0, game[game.size() - 1]->board->playerPhotoB);
    playerLayout[0]->insertWidget(1, game[game.size() - 1]->board->playerB);
    playerLayout[0]->insertWidget(2, game[game.size() - 1]->board->timeLabelB);

    layout->addWidget(game[game.size() - 1]->movehistory);
    adjustSize();

    ChessDatabase *db = myChessDB;
    game[game.size() - 1]->board->setDatabase(db);
    connect(game[game.size() - 1]->board, SIGNAL(madeMove()), engineController, SLOT(go()));
    connect(engineController, SIGNAL(newBestmove(string)), game[game.size() - 1]->board, SLOT(hint(string)));
    connect(game[game.size() - 1]->board, SIGNAL(madeMove(string)), this, SLOT(sendToServer(string)));
    connect(&fics, SIGNAL(newGameID(int)), game[game.size() - 1], SLOT(setGameID(int)));

    connect(game[game.size() - 1]->board, SIGNAL(madeMove()), this, SLOT(updateStatusBar()));
    connect(game[game.size() - 1]->board, SIGNAL(madeMove(string)), this, SLOT(sendToServer(string)));

    //setBoardActive(game.size() - 1);
}

void MainWindow::quit() {
    int ret = QMessageBox::warning(this, "Quit", "Do you really want to quit?", QMessageBox::No | QMessageBox::Yes);
    if(ret == QMessageBox::Yes) close();
}

void MainWindow::ICSgameList() {
    sendToServer("games");
    cout << "sent to server cmd \'games\' " << endl;
    //icgamelist->show();
}

void MainWindow::onICGameListItemclicked(QListWidgetItem* item) {
    string gamestr = item->text().toStdString();
    string idstr = gamestr.substr(0, 3);
    size_t space = idstr.find(" ");
    int i = 0;
    while(space != string::npos) {
        space = idstr.find(" ", space + 1);
        i++;
    }
    idstr.erase(0, i);
    sendToServer("observe " + idstr);
    int gameID = boost::lexical_cast<int>(idstr);
}

void MainWindow::undock() {

}

void MainWindow::scanICS() {
    fics.scanFics();
}

void MainWindow::showPositionTree() {
    posTree = new QTreeWidget;
    posTree->setColumnCount(1);
    posTree->setMinimumSize(350, 600);
    vector<QTreeWidgetItem *> item;
    vector<int> posIDs = myChessDB->getPosIDsByParent(1);

    /* Level 0 */
    item.push_back(new QTreeWidgetItem());
    item[0]->setText(0, "1"); // Initial Position
    posTree->insertTopLevelItem(0, item[0]);
    QObject::connect(posTree, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(itemExpanded(QTreeWidgetItem*)));
    QObject::connect(posTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(showPosition(QTreeWidgetItem*)));
    QTreeWidgetItem* initItem = item[0];

    /* Level 1 */
    for(int i = 0; i < posIDs.size(); i++) {
        QTreeWidgetItem* item = new QTreeWidgetItem(initItem);
        item->setText(0, QString::fromStdString(boost::lexical_cast<string>(posIDs[i])));
    }
    posTree->show();
}

void MainWindow::itemExpanded(QTreeWidgetItem* item) {
    for(int j = 0; j < item->childCount(); j++) {
        vector<int> posIDs = myChessDB->getPosIDsByParent(item->child(j)->text(0).toInt());
        for(int i = 0; i < posIDs.size(); i++) {
            QTreeWidgetItem* newitem = new QTreeWidgetItem(item->child(j));
            newitem->setText(0, QString::fromStdString(boost::lexical_cast<string>(posIDs[i])));
        }
    }
}

void MainWindow::showPosition(QTreeWidgetItem* item) {
    int posID = item->text(0).toInt();
    game[activeBoard]->board->getPositionFromDBByID(posID);
    game[activeBoard]->board->show();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        cout << "left mouse button pressed" << endl;
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        //mimeData->setText(commentEdit->toPlainText());
        drag->setMimeData(mimeData);
        //drag->setPixmap(iconPixmap);

        Qt::DropAction dropAction = drag->exec();
    }
}

void MainWindow::linkClicked(QUrl url) {
    cout << url.toString().toStdString() << " clicked" << endl;
    string moves = url.toString().toStdString();
    int i = 0;
    while(i + 4 <= moves.size()){
        string move = moves.substr(i, 4);
        game[activeBoard]->move(move);
        updateStatusBar();
        i += 5;
    }

}

void MainWindow::updateStatusBar() {
    if(game[activeBoard]->getActiveColor() == 'w')
        statusActiveColor->setText("White to move");
    else
        statusActiveColor->setText("Black to move");
    statusMoveNr->setText(QString::fromStdString("Move " + boost::lexical_cast<string>(game[activeBoard]->getCurrentMoveNr())));
}

void MainWindow::showBestmoveMessage() {
    if(engineB && game[activeBoard]->getActiveColor() == 'b') game[activeBoard]->move(engine.getBestmove());
    else QMessageBox::information(this, "Bestmove", "Bestmove: " + QString::fromStdString(engine.getBestmove()), QMessageBox::Ok);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::warning(this, "Quitting Chessboard", "There are unsaved games. Do you want to save them?", QMessageBox::Close | QMessageBox::Cancel | QMessageBox::Save);
    if (ret == QMessageBox::Save) {
        game[activeBoard]->saveGame();
        //engine.exit();
        event->accept();
    } else if (ret == QMessageBox::Close) {
        //engine.exit();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::setNextPosition(int i) {
    nextPosIndex = i;
}

void MainWindow::unsetEngine(int i) {
    if(i % 2 == 0) engineW = false;
    else engineB = false;
}

void MainWindow::quitGame(int index) {
    /* Remove Board */
    int deleteBoard = index;
    BoardTab->removeTab(deleteBoard);
    delete game[deleteBoard]; // free memory
    game.erase(game.begin()+deleteBoard); // remove game from game array
    BoardBox.erase(BoardBox.begin()+deleteBoard); // remove from BoardBox array

    /* Show another board */
    activeBoard = BoardTab->currentIndex(); // set new active board
    game[activeBoard]->show();
    adjustSize();
}

void MainWindow::createMenu() {
    /* Create Menu */
    fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);
    editMenu = new QMenu(tr("&Edit"), this);
    //menuBar()->addMenu(editMenu);
    gameMenu = new QMenu(tr("&Game"), this);
    menuBar()->addMenu(gameMenu);
    engineMenu = new QMenu(tr("&Engine"), this);
    menuBar()->addMenu(engineMenu);
    databaseMenu = new QMenu(tr("&Database"), this);
    menuBar()->addMenu(databaseMenu);
    icsMenu = new QMenu(tr("&Chess Server"), this);
    menuBar()->addMenu(icsMenu);
    viewMenu = new QMenu(tr("&View"), this);
    menuBar()->addMenu(viewMenu);

    openFile = new QAction(this);
    saveToFile = new QAction(this);
    quitGameAction = new QAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/icons/close.png")), tr("Quit Game"), this);
    quitAction = new QAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Quit ChessBoard"), this);
    fileMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("New Game"), this, SLOT(newGame()), QKeySequence(tr("Ctrl+N", "File|New Game")));
    fileMenu->addAction(openFile);
    fileMenu->addAction(saveToFile);
    //fileMenu->addAction(quitGameAction);
    fileMenu->addAction(quitAction);

    databaseMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Load Game From &Database"), myChessDB, SLOT(showGameSelectDialog()), QKeySequence(tr("Ctrl+D", "File|Database")));
    databaseMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Show Position Tree"), this, SLOT(showPositionTree()), QKeySequence(tr("Ctrl+D", "File|Database")));
    databaseMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Setup Database"), myChessDB, SLOT(setupDB()));

    icsMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("&Connect with Internet Chess Server"), this, SLOT(ICSconnect()), QKeySequence(tr("Ctrl+S", "ICS|Server")));
    icsMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Games on Server"), this, SLOT(ICSgameList()), QKeySequence(tr("Ctrl+S", "ICS|Server")));
    icsMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Scan Internet Chess Server"), this, SLOT(scanICS()), QKeySequence(tr("Ctrl+S", "ICS|Server")));

    gameMenu->addAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("&Flip View"), this, SLOT(), QKeySequence(tr("Ctrl+F", "Game|Flip View")));
    //gameMenu->addAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("&Next Move"), this, SLOT(nextPos()));
    //gameMenu->addAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("&Previous Move"), this, SLOT(prevPos()));
    gameMenu->addAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Set Game ID"), this, SLOT(setGameID()));
    gameMenu->addAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Set Active Color"), this, SLOT(setActiveColor()));
    gameMenu->addAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Duplicate Game"), this, SLOT());

    engineMenu->addAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Undock Engine-Controller"), engineController, SLOT(undock()));

    viewMenu->addAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Undock Gameinfo"), this, SLOT(undock()));
}

/* Set up Signal-Slot Connections */
void MainWindow::connectWidgets() {
    connect(nextMoveAction, SIGNAL(triggered()), this, SLOT(nextPos()));
    connect(prevMoveAction, SIGNAL(triggered()), this, SLOT(prevPos()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

    connect(BoardTab, SIGNAL(currentChanged(int)), this, SLOT(setBoardActive(int)));
    connect(BoardTab, SIGNAL(tabCloseRequested(int)), this, SLOT(quitGame(int)));
    connect(myChessDB, SIGNAL(GameSelected(int)), this, SLOT(checkInputDialog(int)));
    connect(nextCombo, SIGNAL(activated(int)), this, SLOT(setNextPosition(int)));

    /* Buttons */
    connect(next, SIGNAL(clicked()), this, SLOT(nextPos()));
    connect(back, SIGNAL(clicked()), this, SLOT(prevPos()));
    connect(button[0], SIGNAL(clicked()), this, SLOT(prevPos()));
    connect(button[1], SIGNAL(clicked()), this, SLOT(nextPos()));

    /* Status Updates */
    connect(button[4], SIGNAL(clicked()), this, SLOT(updateStatusBar()));
    connect(button[5], SIGNAL(clicked()), this, SLOT(updateStatusBar()));
    connect(engineController, SIGNAL(madeMove()), this, SLOT(updateStatusBar()));
}

void MainWindow::createToolBars() {
    nextMoveAction = new QAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/icons/next.png")), tr("Next Move"), this);
    prevMoveAction = new QAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/icons/prev.png")), tr("Previous Move"), this);
    QToolBar* fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(prevMoveAction);
    fileToolBar->addAction(nextMoveAction);
}
