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
protected:
    string buffer;
    void setThinking(bool state);
    void readEngine();
    string bestmove;
    string fen;
private:
    QProcess* engine;
    QString pathToEngine;
    bool thinking;
signals:
    virtual void stateChanged(int);
    void newOutput(QString);
    virtual void newBestmove();
public slots:
    void showOutput(QString);
    virtual void getValues(QString) = 0;
    virtual void go() = 0;
    virtual void stop() = 0;
};

#endif // ENGINE_H
