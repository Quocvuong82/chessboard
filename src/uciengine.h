#ifndef UCIENGINE
#define UCIENGINE
#include <QThread>
#include <QWidget>
#include <QTextEdit>
#include <QProcess>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <boost/thread.hpp>
#include <iostream>
#include "engine.h"

using namespace std;

class UCIEngine : public Engine {
    Q_OBJECT
public:
    UCIEngine(QString pathToEngine);
    int writeToEngine(string message);
    string readFromEngine(int ReaderID);
    vector<vector<string>> getOtherMoves();
    vector<string> getMultiPV();
    //QTextEdit* output;
    //bool isThinking();
    void setPosition(string fen);
private:
    //QProcess* engine;
    //void readEngine();
    string buffer; // A buffer to store the output of the engine
    vector<size_t> pos; // The last read byte in the buffer

    /* Search Parameters */
    int searchdepth;
    int movetime;
    int mate;
    int nodes;

    bool stopping;
    int currnr = 0;
    int depth = 0; // current search depth
    vector<string> multipvs;
    vector<int> scores;
    vector<int> uniqueScores;
    vector<string> getUniqueMultiPV(int depth);
    static bool isBigger(vector<string> m,vector<string> n);

public slots:
    void showOutput();
    void getValues(QString line);
    void go();
    void stop();
    void setSearchDepth(int value);
    void setMovetime(int value);
    void setMate(int value);
    void setNodes(int value);

signals:
   //void newOutput();
   void newDepth(int);
   void stateChanged(int);
};
#endif // UCIENGINE

