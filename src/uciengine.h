#ifndef ENGINETHREAD
#define ENGINETHREAD
#include <QThread>
#include <QWidget>
#include <QTextEdit>
#include <QProcess>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <boost/thread.hpp>
#include <iostream>

using namespace std;

class UCIEngine : public QObject {
    Q_OBJECT
public:
    UCIEngine(QObject *parent = 0);
    bool stockfish();
    int writeToEngine(string message);
    string readFromEngine(int ReaderID);
    string getBestmove();
    string getOtherMoves();
    vector<string> getMultiPV();
    QTextEdit* output;
    bool isThinking();
    void setPosition(string fen);
private:
    QProcess* engine2;
    void readEngine();
    bool thinking;
    string buffer; // A buffer to store the output of the engine
    vector<size_t> pos; // The last read byte in the buffer
    string bestmove;
    string fen;

    /* Search Parameters */
    int searchdepth;
    int movetime;
    int mate;
    int nodes;

    bool stopping;
    int currnr = 0;
    vector<string> multipvs;
    vector<string> getUniqueMultiPV(int depth);

    /* Pipes to and from engine */
    int	writepipe[2] = {-1,-1},	/* parent -> child */
        readpipe [2] = {-1,-1};	/* child -> parent */
    void readPipe();
    void setThinking(bool state);

public slots:
    void printBestmove();
    void showOutput();
    void getValues();
    void go();
    void stop();
    void setSearchDepth(int value);
    void setMovetime(int value);
    void setMate(int value);
    void setNodes(int value);

signals:
   void newOutput();
   void newBestmove();
   void newDepth(int);
   void stateChanged(int);
};

/* EngineReader reads Output from the UCI-Engine and writes it to the buffer-String */
class EngineReader : public QObject {
    Q_OBJECT
public:
    EngineReader(UCIEngine* parent, QProcess* engine, string* buffer, vector<size_t>* pos);
public slots:
    void process();

signals:
    void finished();
    void error(QString err);
    void newOutput();
private:
    QProcess* engine;
    string* buffer;
    vector<size_t>* pos;
    UCIEngine* parent;
};
#endif // ENGINETHREAD

