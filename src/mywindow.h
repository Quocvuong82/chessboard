#ifndef MYWINDOW
#define MYWINDOW
#include <QWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QComboBox>
#include <QtWidgets>
#include <QLabel>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QTimer>
#include <QVector>
#include <QString>
#include <QThread>
#include <QTabWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <fen.h>
#include <board.h>
#include <iostream>
#include <vector>

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include<fstream>
#include<cstdlib>
#include<sstream>
#include <sys/types.h>  // mkfifo
#include <sys/stat.h>   // mkfifo

#include <string>
#include <algorithm>
#include <boost/lexical_cast.hpp>

#include<fstream>
#include<cstdlib>
#include<sstream>
#include <fcntl.h>
#include <unistd.h>
#include <boost/thread.hpp>

#include "chessdatabase.h"
#include "enginethread.h"
#include "icclient.h"
#include "icgamelist.h"
#include "dbdialog.h"
#include "game.h"
#include "playerlabel.h"
#include "timelabel.h"

using namespace std;

class MyWindow : public QMainWindow {
    Q_OBJECT
public:
    MyWindow(QMainWindow *parent = 0, Qt::WindowFlags flags = 0);
    int writeToStockfish(string message);
private:
    //vector<Board*> board;
    vector<Game*> game;
    int activeBoard = 0;
    ChessDatabase myChessDB = ChessDatabase("localhost", "root", "floppy", "schach");
    bool chessserver;
    bool chessengine;
    bool localboard;
    bool thinkOnMove;
    bool engineW; // Engine plays white
    bool engineB; // Engine plays black
    bool moved; // Has the active player already made a move?
    bool getICGameList; // Receiving Internet Chess Game List?
    ICGameList* icgamelist;
    vector<QListWidgetItem*> item;

    int posID;
    vector<int> posIndex; //Selected position index
    //vector<int> eventIDs;
    //vector<int> gameIDs;
    vector<vector<int>> posIDs;
    char activeColor;
    vector<string> values;

    /* Main Menu */
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *gameMenu;
    QMenu *engineMenu;
    QMenu *icsMenu;
    QMenu *viewMenu;

    QWidget* centralWidget;
    const int NrOfButtons = 6;
    QPushButton* myButton;
    QPushButton* EngineButton;
    QPushButton* RefreshButton;
    QPushButton* next;
    QPushButton* back;
    vector<QPushButton*> button;

    QHBoxLayout *layout;
    QGridLayout *Grid;

    QVBoxLayout* GameInfoLayout;
    QHBoxLayout* ButtonBoxLayout;
    int NrOfBoards = 4;
    vector<QGroupBox*> BoardBox;
    QTabWidget* BoardTab;
    QGroupBox* GameInfoBox;

    //QFrame* selCtrlFrame;
    QFrame* GameFrame;
    QFrame* ButtonFrame;

    /* Player Info */
    vector<string> players;
    vector<int> t;

    QWidget* gameinfo;
    vector<QFrame*> playerFrame;
    vector<PlayerLabel*> player;
    vector<QLabel*> score;
    vector<TimeLabel*> time;
    vector<QHBoxLayout*> playerLayout;
    QTimer* timer;
    //string makeTime(int seconds);

    QLineEdit* input;
    QTextEdit* output;
    QTextEdit* output2;
    QTextEdit* engineOutput;

    DBdialog* dialog;

    void updateBoard();
    bool stockfish();
    string EngineOutput;
    EngineThread engine;
    string bestmove;
    vector<string> scores;
    vector<string> moves;

    ICclient fics; // = ICclient("freechess.org", 5000);
    string outstr;

protected:
  void keyPressEvent(QKeyEvent * e);
public slots:
    // Slot for Control
    void nextPos();
    void prevPos();

    //void setEvent(int value);
    //void setGame(int value);
    void readInput();
    void think();
    void printEngineOutput();
    void readICServer();
    void parseICSOutput(string);
    bool ICSconnect();
    void ICSgameList();
    void onICGameListItemclicked(QListWidgetItem*);
    void sendInputToServer();
    void sendToServer(string);
    void checkInputDialog();
    void checkInputDialog(int);
    void clocks();
    void setBoardActive(int);
    void newGame();
    void SquareClicked(int);
    void EnginePlayBlack();
    void EnginePlayWhite();
    void quit();
    void setGameID();
    void undock();
    void scanICS();
};

#endif // MYWINDOW

