#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QTextEdit>
#include <QProcess>
#include <string>

using namespace std;
class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QString pathToEngine, QObject *parent = 0);
    virtual int writeToEngine(string message);
    QTextEdit* output;
    bool isThinking();
    string getBestmove();
    void setPosition(string fen);
    virtual vector<vector<string>> getOtherMoves() = 0;
    int getSearchDepth();
    int getMovetime();
    int getMate();
    int getNodes();
protected:
    string buffer;
    void setThinking(bool state);
    void readEngine();
    string bestmove;
    string fen;
    int depth = 0; // current search depth

    /* Search Parameters */
    int searchdepth;
    int movetime;
    int mate;
    int nodes;
private:
    QProcess* engine;
    QString pathToEngine;
    bool thinking;
signals:
    virtual void stateChanged(int);
    void newOutput(QString);
    virtual void newBestmove();
    virtual void newDepth(int);
public slots:
    void showOutput(QString);
    virtual void getValues(QString) = 0;
    virtual void go() = 0;
    virtual void stop() = 0;
    virtual void setSearchDepth(int value) = 0;
    virtual void setMovetime(int value) = 0;
    virtual void setNodes(int value) = 0;
};

#endif // ENGINE_H
