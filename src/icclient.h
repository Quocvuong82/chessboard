#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "fen.h"
#include "board.h"
#include "chessdatabase.h"
#include <fcntl.h>
#include <QObject>
#include <QTextEdit>
#include <QLineEdit>
#ifndef ICCLIENT
#define ICCLIENT
using namespace std;
using namespace boost;

class ICclient : public QObject {
    Q_OBJECT
public:
    ICclient(QObject *parent = 0);
    //ICclient(string hostname, int portno);
    bool connect();
    bool history();
    bool examine();
    int saveGameToDB(string handle, int gameID);
    int saveGames(string handle);
    void scanFics();
    string readFromServer();
    bool writeSocket(string message);
    int pos;
    bool isunread();
    bool isunread2();
    bool isConnected();
private:
    bool connected = false;
    unsigned int latency = 10000;
    ChessDatabase myChessDB;
    int p = 0; // new positions added to database
    size_t Rpos; // Read-Position
    size_t Rbuffer;
    bool newServerData; // server wrote datastring to client
    int sockfd;
    bool AUTOLOAD;
    bool LOADFILTERD;
    string host;
    int portno;
    string buffer;
    QString outstr;
    QTextEdit* output;
    QWidget* browser;
    QLineEdit* input;

    string readSocket();
    string readLine();
    void printFilteredServerMessages(regex filter);
    string getFilteredServerMessage(regex filter);
    string getServerAnswer();
    void error(const char *msg);
    void bufferManager();
    void clearBuffer();
signals:
   void newOutput();
   void unread();
   void newLine(QString);
   void newGameID(int);
public slots:
   void emitSignal();
   void getGame();
   void readInput();
   void printOutput(QString line);
};

#endif // ICCLIENT

