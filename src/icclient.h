#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
#include <fcntl.h>
#include <QObject>
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
    bool saveGames(string handle);
    string readFromServer();
    bool writeSocket(string message);
    int pos;
    bool isunread();
private:
    bool connected = true;
    unsigned int latency = 10000;
    size_t Rpos; // Read-Position
    size_t Rbuffer;
    bool newServerData; // server wrote datastring to client
    int sockfd;
    bool AUTOLOAD;
    bool LOADFILTERD;
    string host;
    int portno;
    string buffer;

    string readSocket();
    string readLine();
    void printFilteredServerMessages(regex filter);
    string getFilteredServerMessage(regex filter);
    string getServerAnswer();
    void error(const char *msg);
    void bufferManager();
signals:
   void newOutput();
   void unread();
public slots:
   void emitSignal();
};

#endif // ICCLIENT

