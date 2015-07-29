#include "mywindow.h"
#include <QtCore>
#include <QtGui>
#include "fen.h"
#include <QAction>
#include"enginethread.h"

MyWindow::MyWindow(QMainWindow *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags) {

    /* Boolean Variables */
    chessserver = false;
    getICGameList = false;
    chessengine = false;
    localboard = true;
    thinkOnMove = false;
    engineB = false;
    engineW = false;
    moved = false;


    this->setWindowTitle("Chessboard 0.5");
    this->resize(900, 400);

    dialog = new DBdialog(myChessDB);
    dialog->setAttribute(Qt::WA_QuitOnClose);
    QObject::connect(dialog, SIGNAL(finished(int)), this, SLOT(checkInputDialog(int)));
    QObject::connect(dialog, SIGNAL(rejected()), this, SLOT(checkInputDialog()));

    fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);
    editMenu = new QMenu(tr("&Edit"), this);
    menuBar()->addMenu(editMenu);
    gameMenu = new QMenu(tr("&Game"), this);
    menuBar()->addMenu(gameMenu);
    engineMenu = new QMenu(tr("&Engine"), this);
    menuBar()->addMenu(engineMenu);
    icsMenu = new QMenu(tr("&ICS"), this);
    menuBar()->addMenu(icsMenu);
    viewMenu = new QMenu(tr("&View"), this);
    menuBar()->addMenu(viewMenu);

    fileMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("New Game"), this, SLOT(newGame()), QKeySequence(tr("Ctrl+N", "File|New Game")));
    fileMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Load Game From &Database"), dialog, SLOT(exec()), QKeySequence(tr("Ctrl+D", "File|Database")));
    fileMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Quit"), this, SLOT(quit()), QKeySequence(tr("Ctrl+Q", "Quit")));
    engineMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("&Think on position"), this, SLOT(think()), QKeySequence(tr("Ctrl+T", "Engine|Think")));
    engineMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Play &Black"), this, SLOT(EnginePlayBlack()), QKeySequence(tr("Ctrl+B", "Engine|Play Black")));
    engineMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Play &White"), this, SLOT(EnginePlayBlack()), QKeySequence(tr("Ctrl+B", "Engine|Play White")));

    icsMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("&Connect with Internet Chess Server"), this, SLOT(ICSconnect()), QKeySequence(tr("Ctrl+S", "ICS|Server")));
    icsMenu->addAction( QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Games on Server"), this, SLOT(ICSgameList()), QKeySequence(tr("Ctrl+S", "ICS|Server")));

    gameMenu->addAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("&Flip View"), this, SLOT(), QKeySequence(tr("Ctrl+F", "Game|Back Move")));
    gameMenu->addAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("&Next Move"), this, SLOT(nextPos()));
    gameMenu->addAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("&Previous Move"), this, SLOT(prevPos()));
    gameMenu->addAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Set Game ID"), this, SLOT(setGameID()));

    viewMenu->addAction(QIcon(QString("%1%2") .arg(QCoreApplication::applicationDirPath()) .arg("/images/page_white.png")), tr("Undock Gameinfo"), this, SLOT(undock()));

    /* Initialize Games and Boards */
    for(int i = 0; i < NrOfBoards; i++) {
        game.push_back(new Game());
        for(int j = 0; j < game[i]->board->squares.size(); j++) {
            QObject::connect(game[i]->board->squares[j], SIGNAL(clicked(int)), this, SLOT(SquareClicked(int)));
            QObject::connect(game[i]->board->squares[j], SIGNAL(dropped(int, int)), this, SLOT(SquareDropped(int, int)));
        }
    }

    posID = 1;

    /* Buttons */
    next = new QPushButton("next");
    back = new QPushButton("back");
    input = new QLineEdit();
    EngineButton = new QPushButton("Think");
    for(int i = 0; i < NrOfButtons; i++) {
        button.push_back(new QPushButton(QString("B").append(QString(QString::fromStdString(boost::lexical_cast<string>(i))))));
        if(i != 2) button[i]->setFixedSize(40,25);
    };
    EngineButton->setFixedSize(50,25);
    button[4]->setText("<");
    button[5]->setText(">");
    button[0]->setText("R ICS");
    button[1]->setText("games");
    button[2]->setText("Pos Tree");
    button[3]->setText("Scan ICS");


    /* Signal - Slot Connections */  
    QObject::connect(next, SIGNAL(clicked()), this, SLOT(nextPos()));
    QObject::connect(back, SIGNAL(clicked()), this, SLOT(prevPos()));
    QObject::connect(input, SIGNAL(returnPressed()), this, SLOT(readInput()));
    QObject::connect(EngineButton, SIGNAL(clicked()), this, SLOT(think()));
    QObject::connect(button[0], SIGNAL(clicked()), this, SLOT(readICServer()));
    QObject::connect(button[1], SIGNAL(clicked()), this, SLOT(ICSgameList()));
    QObject::connect(button[2], SIGNAL(clicked()), this, SLOT(showPositionTree()));
    QObject::connect(button[3], SIGNAL(clicked()), this, SLOT(scanICS()));
    QObject::connect(button[4], SIGNAL(clicked()), this, SLOT(prevPos()));
    QObject::connect(button[5], SIGNAL(clicked()), this, SLOT(nextPos()));


/* -------------------------------------------------------------------------------------- */
/*   D e f i n e  L a y o u t                                                             */
/* -------------------------------------------------------------------------------------- */

    /* Tabbed Board - Layout */
    BoardTab = new QTabWidget();
    QGroupBox* b = new QGroupBox;
    b->setFixedSize(300,300);
    for(int i = 0; i < Game::getNrOfGames(); i++) {
        BoardBox.push_back(new QGroupBox);
        BoardBox[i]->setFixedSize(576,576);
        BoardTab->addTab(BoardBox[i], "Game " + QString::fromStdString(boost::lexical_cast<string>(i + 1)));
        BoardBox[i]->setLayout(game[i]->board->Grid);
    }
    QObject::connect(BoardTab, SIGNAL(currentChanged(int)), this, SLOT(setBoardActive(int)));

    /* Buttons */
    ButtonFrame = new QFrame();
    ButtonBoxLayout = new QHBoxLayout;
    ButtonFrame->setLayout(ButtonBoxLayout);
    ButtonBoxLayout->addWidget(EngineButton);
    for(int i = 0; i < NrOfButtons; i++) {
        ButtonBoxLayout->addWidget(button[i]);
    }

    /* Player Info */
    for(int i = 0; i < 2; i++) {
        playerFrame.push_back(new QFrame());
        playerLayout.push_back(new QHBoxLayout);
        player.push_back(new PlayerLabel);
        player[i]->setText("Player " + boost::lexical_cast<string>(i));
        time.push_back(new TimeLabel);
        score.push_back(new QLabel);
        playerLayout[i]->addWidget(player[i]);
        playerLayout[i]->addWidget(time[i]);
        playerLayout[i]->addWidget(score[i]);
        playerFrame[i]->setLayout(playerLayout[i]);  
    }

    /* Style Player and Time Labels */
    player[0]->setColor("black");
    player[1]->setColor("white");
    time[0]->setColor("black");
    time[1]->setColor("white");
    time[0]->setTime(1800);
    for(int i = 0; i < 2 * Game::getNrOfGames(); i++) {
        players.push_back("Player " + boost::lexical_cast<string>(i));
        t.push_back(1800);
    }

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(clocks()));
    timer->start(1000);

    /* Output - Boxes */
    output = new QTextEdit();
    output->setReadOnly(true);
    output2 = new QTextEdit();
    output2->setReadOnly(true);
    engineOutput = output2;
    engineOutput->show();
    engineOutput->setMinimumWidth(400);
    engineOutput->setWindowTitle("Engine");

    /* Game Info */
    gameinfo = new QWidget;
    QVBoxLayout* gameinfoLayout = new QVBoxLayout;
    gameinfo->setLayout(gameinfoLayout);
    gameinfoLayout->addWidget(playerFrame[0]);
    gameinfoLayout->addWidget(playerFrame[1]);

    GameInfoLayout = new QVBoxLayout;
    GameInfoLayout->addWidget(gameinfo);
    //GameInfoLayout->addWidget(playerFrame[0]);
    //GameInfoLayout->addWidget(playerFrame[1]);
    GameInfoLayout->addWidget(ButtonFrame);
    GameInfoLayout->addWidget(input);
    GameInfoLayout->addWidget(output);
    GameInfoLayout->addWidget(output2);
    GameInfoBox = new QGroupBox("Game Info");
    GameInfoBox->setLayout(GameInfoLayout);

    /* Main Layout */
    layout = new QHBoxLayout;
    layout->addWidget(BoardTab);
    layout->addWidget(GameInfoBox);
    layout->setMargin(0);

    centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    this->setCentralWidget(centralWidget);
    this->setFocusPolicy(Qt::StrongFocus);

/* -------------------------------------------------------------------------------------- */


    /* Get position data from Database and display it on the GUI board */
    for(int i = 0; i < Game::getNrOfGames(); i++) {
        //game[i]->board->getPositionFromDBByID(posID);
        game[i]->board->show(); // Write position to squares (QLabels)
        posIDs.push_back(vector<int> ());
        posIndex.push_back(0);
    }

    /* Connect Engine-Thread with Output-TextEdit Object */
    QObject::connect(&engine, SIGNAL(newOutput()), this, SLOT(printEngineOutput()));

    engine.start();
    engine.stockfish();
    engine.writeToEngine("setoption name Threads value 4");

    QObject::connect(&fics, SIGNAL(unread()), this, SLOT(readICServer()));
    output2->setMinimumHeight(50);
    output2->resize(100,100);


}  
void MyWindow::setBoardActive(int index) {
    cout << "board " + boost::lexical_cast<string>(index) + " set active" << endl;
    activeBoard = index;
    //cout << players[activeBoard * 2] << endl;
    //cout << players[activeBoard * 2 + 1] << endl;

    player[0]->setText(players[activeBoard * 2 + 1]);
    player[1]->setText(players[activeBoard * 2]);
    time[1]->setTime(t[activeBoard * 2 + 1]);
    time[0]->setTime(t[activeBoard * 2]);
    /*time[0]->setText("<font size=20 color=black><b>" + QString::fromStdString(makeTime(t[activeBoard * 2 + 1])) + "</b></font>");
    time[1]->setText("<font size=20 color=white><b>" + QString::fromStdString(makeTime(t[activeBoard * 2])) + "</b></font>");*/
    game[activeBoard]->board->show();
}

void MyWindow::checkInputDialog() {
    int val = dialog->result();
    if( val == QDialog::Accepted) {
        cout << "Akzeptiert!" << endl;
    } else if (val == QDialog::Rejected) {
        cout << "Abgelehnt!" << endl;
    }
}

void MyWindow::checkInputDialog(int gameID) {
    if (!dialog->result() == QDialog::Rejected) {
        localboard = true;
        posIndex[activeBoard] = 0;
        posIDs[activeBoard] = myChessDB.getPosIDsByGameID(gameID);
        //posID = posIDs[0];
        vector<string> p = myChessDB.getPlayersByGameID(gameID);
        for(int i = 0; i < 2; i++) {
            players[activeBoard * 2 + i] = p[i];
        }
        cout << players[activeBoard * 2] << endl;
        cout << players[activeBoard * 2 + 1] << endl;
        player[0]->setText(players[activeBoard * 2]);
        player[1]->setText(players[activeBoard * 2 + 1]);

        updateBoard();
    }
}

void MyWindow::keyPressEvent(QKeyEvent *e)
{
   switch(e->key()) {
   case Qt::Key_Right:
       nextPos();
       break;
   case Qt::Key_Left:
       prevPos();
       break;
   }
}

void MyWindow::nextPos() {
    if(chessserver && examining && !localboard) {
       fics.writeSocket("forward\n");
    } else {
        if(posIDs[activeBoard].size() > 0 && (posIndex[activeBoard] + 1) < posIDs[activeBoard].size()) {
            posIndex[activeBoard]++;
            updateBoard();
        }
    }
    if(thinkOnMove) think();
}

void MyWindow::prevPos() {
    if(chessserver && examining && !localboard) {
       fics.writeSocket("backward\n");
    } else {
        if(posIndex[activeBoard]>0) {
            posIndex[activeBoard]--;
            updateBoard();
        }
    }
    if(thinkOnMove) think();
}

/* Engine Slots                                                               */
/* -------------------------------------------------------------------------- */

void MyWindow::EnginePlayBlack() {
   engineB = true;
   if(game[activeBoard]->getActiveColor() == 'b') think();
}

void MyWindow::EnginePlayWhite() {
   engineW = true;
   if(game[activeBoard]->getActiveColor() == 'w') think();
}

void MyWindow::think() {
    /* Create Command with Fen-String */
    string command;
    command = "position fen " + game[activeBoard]->board->getFenstring();
    command.append("\ngo");
    cout << command << endl;

    output2->clear(); // Clear the OutputBox
    scores.clear();
    moves.clear();
    bestmove.clear();
    moved = false;
    engine.writeToEngine(command); //writeToStockfish(command);
}

void MyWindow::printEngineOutput() {
    string outstr = engine.readFromEngine();


    /* get bestmove */
    int p = outstr.find("bestmove ");
    int e = outstr.find(" ", p + 9);
    if(p != string::npos) {
        bestmove = outstr.substr(p + 9, e- p - 9);
    }

    /* get score of the best move */
    size_t sc = 0, pv;
    sc = outstr.find("score ");
    while(sc < outstr.length()) {
        pv = outstr.find(" pv ", sc + 1);
        int l = 1;
        if(sc < outstr.length()) {
            //cout << outstr.length() << " " << sc << " " << pv << endl;
            if((sc + 9) < outstr.length()) {
                //string str = outstr.substr(sc + 10);
                l = outstr.substr(sc + 9).find(" ");
            }
            //cout << "score: ";
            if((sc + 9 + l) < outstr.length()) {
                //cout << outstr.substr(sc + 9, l) << endl;
                scores.push_back(outstr.substr(sc + 9, l));
                if(pv != string::npos) moves.push_back(outstr.substr(pv + 4, 4));
            }
        }
        sc = outstr.find("score ", sc + 1);
    }

    /* check who is active player */
    int i = 0;
    if(game[activeBoard]->getActiveColor() == 'b') i = 1;

    /* if we know which move is best, get it's score */
    if(bestmove.size() > 0) {
        for(int j = 0; j < scores.size() && j < moves.size(); j++) {
            if(moves[j] == bestmove) score[i]->setText(QString::fromStdString(scores[j]));
        }
        /* we have a best move --> we can make this move (if engine is playing) */
        if((engineB && game[activeBoard]->getActiveColor() == 'b' && !moved) || (engineW && game[activeBoard]->getActiveColor() == 'w' && !moved)) {
            cout << "make move: " << bestmove << endl;
            game[activeBoard]->board->move(bestmove);
            moved = true;
        }
        if(moved && thinkOnMove) think();
    }


    /* print engine output to output box */
    engineOutput->setText(engineOutput->toPlainText().append(QString::fromStdString(outstr)));
    engineOutput->verticalScrollBar()->setValue(engineOutput->verticalScrollBar()->maximum());
}

void MyWindow::readInput() {
    if(chessserver) sendInputToServer(); else {
        /* Make a move */
        game[activeBoard]->board->move(input->text().toStdString());

        /* Aktivate oppent (chess engine) */
        if((engineB && game[activeBoard]->getActiveColor() == 'b') || (engineW && game[activeBoard]->getActiveColor() == 'w')) think();
    }
    input->clear();
}

/* Internet Chess Server Slots                                                */
/* -------------------------------------------------------------------------- */

bool MyWindow::ICSconnect() {
    chessserver = true;
    localboard = false;
    fics.connect();
    output->clear();
    icgamelist = new ICGameList();
    QObject::connect(icgamelist->list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onICGameListItemclicked(QListWidgetItem*)));
    //QObject::connect(icgamelist, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onICGameListItemclicked(QListWidgetItem*)));
    //output->setText(QString::fromStdString(fics.readSocket()));
}

void MyWindow::sendInputToServer() {
    //readICServer();
    output->setText(output->toPlainText().append(input->text()));
    output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum());
    sendToServer(input->text().toStdString());
}

void MyWindow::sendToServer(string msg) {
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
    fics.writeSocket(msg);//.append("\n"));
    //fics.writeSocket("help\n");
}

void MyWindow::setGameID() {
    bool ok;
    int gameID = QInputDialog::getInt(
                this, "Set Game ID", "Enter a new Game ID", 10, 0 , 999, 1, &ok);
    if(ok) game[activeBoard]->setGameID(gameID);

}

void MyWindow::readICServer() {
    string line = fics.readFromServer();
    parseICSOutput(line);
    outstr += line;
    if(!fics.isunread()) {
        output->setText(QString::fromStdString(outstr));
        output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum());
    }
    /*while(chessserver) {
       string outstr = fics.readFromServer();
       while(outstr.size() > 0) {
           //parseICSOutput(outstr);
           cout << outstr;
           outstr = fics.readFromServer();
        }
       usleep(1000);
    }*/
}

void MyWindow::parseICSOutput(string outstr) {
    //string outstr = fics.readFromServer();
    string position;
    vector<string> fenstrings(13);

    size_t g = outstr.find("games");
    //boost::regex expr("[0-9]+[ ][0-9+-]+[ ]");
    //boost::regex expr("[0-9]+[ ][0-9+-]+[ ][A-z]+[ ][0-9+-]+[ ][A-z]+");
    boost::regex expr("[0-9]+[ ][0-9+-]+[ ][A-z]+[ ]+[0-9+-]+[ ][A-z]+[ ]+[\[][ ]?");

    boost::smatch m;
    if(boost::regex_search(outstr, m, expr)) {
        //cout << "REGEX MATCH" << endl;
        getICGameList = true;
        /*size_t gEnd = outstr.find("games displayed.");
        if(gEnd != string::npos) {
            getICGameList = false;
            icgamelist->show();
        } else {*/
            item.push_back(new QListWidgetItem);
            //cout << outstr << "|";
            item[item.size() - 1]->setText(QString::fromStdString(outstr.substr(0, outstr.size() - 1)));
            icgamelist->addItem(item[item.size() - 1]);
            //icgamelist->show();
            /*size_t newline = outstr.find("\n");
            size_t begin = g;
            size_t end = outstr.find("games displayed.");

            int i = 0;
            while(newline < end) {
                item.push_back(new QListWidgetItem);
                cout << outstr.substr(begin, newline - begin) << endl;
                item[i]->setText(QString::fromStdString(outstr.substr(begin, newline - begin)));
                icgamelist->addItem(item[i]);
                begin = newline + 1;
                newline = outstr.find("\n", newline + 1);
                i++;
                if(i > 1000) break;
            }*/
            //ICSgameList();
        //}
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
        //cout << "gameID: " << values[16] << endl;
        //cout << fenstrings[0] << " " << fenstrings[1] << fenstrings[2] << endl;
        bool isSet = false;
        for(int i = 0; i < Game::getNrOfGames(); i++) {
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
            for(int i = 0; i < 2; i++){
                players[activeBoard * 2 + i] = values[17 + i];
                t[activeBoard * 2 + i] = boost::lexical_cast<int>(values[24 + 1 - i]);

            }
            time[1]->setTime(boost::lexical_cast<int>(values[24]));
            time[0]->setTime(boost::lexical_cast<int>(values[25]));
            player[1]->setText(values[17]);
            player[0]->setText(values[18]);
            game[activeBoard]->board->show();
            if(thinkOnMove) think();
        }

    }

    //output->setText(output->toPlainText().append(QString::fromStdString(outstr)));
    //output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum());
}

/*string MyWindow::makeTime(int seconds) {
    int m;
    string timestr;
    m = seconds / 60;
    seconds = seconds - m * 60;
    if(m < 10) timestr.append("0");
    timestr.append(boost::lexical_cast<string>(m));
    timestr.append(":");
    if(seconds < 10) timestr.append("0");
    timestr.append(boost::lexical_cast<string>(seconds));
    return timestr;
}*/

void MyWindow::clocks() {
    /* Update time for every game every second */
    for(int j = 0; j < Game::getNrOfGames(); j++) {
        int i = 0;
        if(game[j]->getActiveColor() == 'b') i = 1;
        t[j * 2 + i]--;
        if(j == activeBoard) time[i]->setTime(t[j * 2 + i]);
        //cout << (j * 2 + i) << endl;
    }
    /*for(int i = 0; i < 2* Game::getNrOfGames(); i++) {
        cout << boost::lexical_cast<string>(t[i]) << " ";
    }
    cout << endl;*/
}

void MyWindow::updateBoard() {
    //posIDLabel->setText(QString::number(posIDs[posIndex]));
    game[activeBoard]->board->getPositionFromDBByID(posIDs[activeBoard][posIndex[activeBoard]]);
    //fenstring->setText(QString::fromStdString(board->getFenstring()));
    game[activeBoard]->board->show();
}

void MyWindow::newGame() {
    cout << "new game " << endl;
    game.push_back(new Game());
    game[Game::getNrOfGames() - 1]->board->getPositionFromDBByID(1);

    /* Connect Square-Signals to MainWindow-Slots */
    for(int j = 0; j < game[Game::getNrOfGames() - 1]->board->squares.size(); j++) {
        QObject::connect(game[Game::getNrOfGames() - 1]->board->squares[j], SIGNAL(clicked(int)), this, SLOT(SquareClicked(int)));
        QObject::connect(game[Game::getNrOfGames() - 1]->board->squares[j], SIGNAL(dropped(int, int)), this, SLOT(SquareDropped(int, int)));
    }

    game[Game::getNrOfGames() - 1]->board->show(); // Write position to squares (QLabels)
    posIDs.push_back(vector<int> ());
    posIndex.push_back(0);
    BoardBox.push_back(new QGroupBox);
    BoardBox[BoardBox.size() - 1]->setFixedSize(576,576);
    BoardTab->addTab(BoardBox[BoardBox.size() - 1], "Game " + QString::fromStdString(boost::lexical_cast<string>(BoardBox.size())));
    BoardBox[BoardBox.size() - 1]->setLayout(game[BoardBox.size() - 1]->board->Grid);
    for(int i = 0; i < 2; i++) {
        players.push_back("Player " + boost::lexical_cast<string>(2 * (Game::getNrOfGames() - 1) + i));
        t.push_back(0);
    }
    BoardTab->setCurrentIndex(Game::getNrOfGames() - 1);
    setBoardActive(Game::getNrOfGames() - 1);
}

void MyWindow::SquareClicked(int id) {
    /*int y = id / 8 + 1;
    int x = id % 8;
    string cmd;
    cmd.push_back(static_cast<char>(x + 97));
    cmd.push_back(static_cast<char>(y + 48));
    cout << "Square " << cmd << " clicked " << endl;
    if(clickcmd) {
        QString txt = input->text();
        input->setText(txt.append(QString::fromStdString(cmd)));
        clickcmd = false;
        readInput();
    } else {
        input->setText(QString::fromStdString(cmd));
        clickcmd = true;
    }*/
}

void MyWindow::SquareDropped(int target, int source) {
    int y = source / 8 + 1;
    int x = source % 8;
    string cmd;
    cmd.push_back(static_cast<char>(x + 97));
    cmd.push_back(static_cast<char>(y + 48));
    y = target / 8 + 1;
    x = target % 8;
    cmd.push_back(static_cast<char>(x + 97));
    cmd.push_back(static_cast<char>(y + 48));
    //Board b; b.move(); b.sh
    cout << "SquareDropped: " << cmd << endl;
    //if(chessserver) game[activeBoard]->board->move(cmd);
    //input->setText(QString::fromStdString(cmd));
    //readInput();
    if(chessserver) {
        output->setText(output->toPlainText().append(QString::fromStdString(cmd)));
        output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum());
        sendToServer(cmd);
        game[activeBoard]->board->move(cmd);
    } else {
        /* Make a move */
        game[activeBoard]->board->move(cmd);

        /* Aktivate oppent (chess engine) */
        if((engineB && game[activeBoard]->getActiveColor() == 'b') || (engineW && game[activeBoard]->getActiveColor() == 'w')) think();
    }
}

void MyWindow::quit() {
    int ret = QMessageBox::warning(this, "Quit", "Do you really want to quit?", QMessageBox::No | QMessageBox::Yes);
    if(ret == QMessageBox::Yes) close();
}

void MyWindow::ICSgameList() {
    input->setText(QString::fromStdString("games"));
    cout << "sent to server games " << endl;
    //icgamelist->show();
}

void MyWindow::onICGameListItemclicked(QListWidgetItem* item) {
    string gamestr = item->text().toStdString();
    string idstr = gamestr.substr(0, 3);
    size_t space = idstr.find(" ");
    int i = 0;
    while(space != string::npos) {
        space = idstr.find(" ", space + 1);
        i++;
    }
    idstr.erase(0, i);
    cout <<  idstr << endl;
    input->setText(QString::fromStdString("observe " + idstr));
    sendInputToServer();
    int gameID = boost::lexical_cast<int>(idstr);
}

void MyWindow::undock() {
    //GameInfoLayout->removeWidget(gameinfo);
    gameinfo->setParent(0);
    gameinfo->show();
}

void MyWindow::scanICS() {
    fics.scanFics();
}

void MyWindow::showPositionTree() {
    posTree = new QTreeWidget;
    posTree->setColumnCount(1);
    posTree->setMinimumSize(350, 600);
    vector<QTreeWidgetItem *> item;
    vector<int> posIDs = myChessDB.getPosIDsByParent(1);

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

void MyWindow::itemExpanded(QTreeWidgetItem* item) {
    for(int j = 0; j < item->childCount(); j++) {
        vector<int> posIDs = myChessDB.getPosIDsByParent(item->child(j)->text(0).toInt());
        for(int i = 0; i < posIDs.size(); i++) {
            QTreeWidgetItem* newitem = new QTreeWidgetItem(item->child(j));
            newitem->setText(0, QString::fromStdString(boost::lexical_cast<string>(posIDs[i])));
        }
    }
}

void MyWindow::showPosition(QTreeWidgetItem* item) {
    int posID = item->text(0).toInt();
    game[activeBoard]->board->getPositionFromDBByID(posID);
    game[activeBoard]->board->show();
}

void MyWindow::mousePressEvent(QMouseEvent *event) {
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
