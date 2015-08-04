#ifndef ENGINETHREAD
#define ENGINETHREAD
#include <QThread>
#include <QWidget>
#include <QTextEdit>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <boost/thread.hpp>
#include <iostream>

using namespace std;

class EngineThread : public QThread {
    Q_OBJECT
public:
    EngineThread(QObject *parent = 0);
    void emitMySignal();
    bool stockfish();
    int writeToEngine(string message);
    string readFromEngine(int ReaderID);
    string getBestmove();
    vector<string> getMultiPV();
    QTextEdit* output;
private:
    /* Pipes to and from engine */
    int	writepipe[2] = {-1,-1},	/* parent -> child */
        readpipe [2] = {-1,-1};	/* child -> parent */
    string buffer; // A buffer to store the output of the engine
    vector<size_t> pos; // The last read byte in the buffer
    void readPipe();
    string bestmove;
    void getValues();
    int currnr = 0;
    string screen; // MultiPV Output
    vector<string> multipvs;
public slots:
    void printBestmove();
    void showOutput();
signals:
   void newOutput();
   void newBestmove();
};
#endif // ENGINETHREAD

