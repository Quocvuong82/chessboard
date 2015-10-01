#ifndef MAINWINDOW
#define MAINWINDOW
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
#include <QSlider>
#include <QThread>
#include <QTabWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QTreeWidget>
#include <QWebView>
#include <QObject>
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

#include "qchessbase.h"
#include "uciengine.h"
#include "icclient.h"
#include "icgamelist.h"
#include "game.h"
#include "playerlabel.h"
#include "timelabel.h"
#include "enginecontroller.h"
using namespace std;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QMainWindow *parent = 0, Qt::WindowFlags flags = 0);
private:
    vector<Game*> game;
    int activeBoard = 0;
    //QChessBase myChessDB = QChessBase("localhost", "root", "floppy", "schach");
    QChessBase* myChessDB;// = QChessBase();
    bool chessserver;
    bool examining = false;
    bool chessengine;
    bool localboard;
    bool engineW; // Engine plays white
    bool engineB; // Engine plays black
    bool moved; // Has the active player already made a move?
    bool getICGameList; // Receiving Internet Chess Game List?
    ICGameList* icgamelist;
    vector<QListWidgetItem*> item;

    int posID;
    vector<int> posIndex; //Selected position index
    int nextPosIndex = 0;
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
    QMenu *databaseMenu;

    /* Status Bar */
    QLabel* statusActiveColor;
    QLabel* statusMoveNr;

    QWidget* centralWidget;
    const int NrOfButtons = 7;
    QPushButton* myButton;
    QPushButton* EngineButton;
    QPushButton* RefreshButton;
    QPushButton* next;
    QComboBox* nextCombo;
    QPushButton* back;
    vector<QPushButton*> button;

    QHBoxLayout *layout;
    QGridLayout *Grid;

    QVBoxLayout* GameInfoLayout;
    QHBoxLayout* ButtonBoxLayout;
    int NrOfBoards = 4; // how many boards will be initialized
    vector<QFrame*> BoardBox;
    QTabWidget* BoardTab;
    QGroupBox* GameInfoBox;
    QVBoxLayout* boardSliderBox;

    /* Player Info */
    QVBoxLayout* playersLayout;
    vector<QFrame*> playerFrame;
    vector<QLabel*> score;
    vector<QHBoxLayout*> playerLayout;

    QLineEdit* input;
    QTextEdit* output;
    QTextEdit* output2;
    QTextEdit* engineOutput;
    QWebView* engineView;

    QSlider* slider;

    void updateBoard();
    bool stockfish();
    string EngineOutput;
    UCIEngine engine;
    EngineController* engineController;
    string bestmove;
    vector<string> scores;
    vector<string> moves;
    vector<string> currmoves;
    vector<string> multipvs;
    vector<int> currmovenr;
    int currnr = 0;
    ICclient fics; // = ICclient("freechess.org", 5000);
    string outstr;

    QTreeWidget* posTree;

    bool clickcmd = false;

    void getNextPositions();

    /* Style */
    QString StyleSheet;

protected:
  void keyPressEvent(QKeyEvent *e);
  void mousePressEvent(QMouseEvent *event);
  void closeEvent(QCloseEvent *event);
public slots:
    // Slot for Control
    void nextPos();
    void prevPos();

    void readInput();

    void readICServer();
    void parseICSOutput(string);
    bool ICSconnect();
    void ICSgameList();
    void onICGameListItemclicked(QListWidgetItem*);
    void sendInputToServer();
    void sendToServer(string);

    void checkInputDialog();
    void checkInputDialog(int);
    void setBoardActive(int);
    void newGame();
    void SquareDropped(int, int);
    void quit();
    void setGameID();
    void setActiveColor();
    void undock();
    void scanICS();
    void showPositionTree();
    void itemExpanded(QTreeWidgetItem*);
    void showPosition(QTreeWidgetItem*);
    void linkClicked(QUrl);
    void updateStatusBar();
    void showBestmoveMessage();
    void setNextPosition(int);
    void unsetEngine(int i);
    void quitGame();
};

#endif // MAINWINDOW

