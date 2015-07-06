#ifndef ENGINETHREAD
#define ENGINETHREAD
#include <QThread>
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
    string readFromEngine();
private:
    /* Pipes to and from engine */
    int	writepipe[2] = {-1,-1},	/* parent -> child */
        readpipe [2] = {-1,-1};	/* child -> parent */
    string buffer; // A buffer to store the output of the engine
    size_t pos; // The last read byte in the buffer
    void readPipe();

signals:
   void newOutput();
};
#endif // ENGINETHREAD

