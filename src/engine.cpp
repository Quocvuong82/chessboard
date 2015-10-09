#include <QScrollBar>
#include <QDebug>
#include "engine.h"
#include "iostream"
#include <unistd.h>
#include <boost/thread.hpp>
using namespace std;

Engine::Engine(QString pathToEngine, QObject *parent) : QObject(parent)
{
    engine = new QProcess();
    engine->start(pathToEngine);
    thinking = false;

    //connect(this, SIGNAL(newOutput(QString)), this, SLOT(showOutput(QString)));
    //connect(this, SIGNAL(newOutput(QString)), this, SLOT(getValues(QString)));

    setThinking(false);
    output = new QTextEdit();
    output->setReadOnly(true);
    bestmove = "";
}

int Engine::writeToEngine(string message) {
    return engine->write((message + "\n").c_str(), qstrlen((message + "\n").c_str()));
}

bool Engine::isThinking() {
    return thinking;
}

void Engine::setThinking(bool state) {
    thinking = state;
    qDebug() << "Engine:" << "state changed" << state;
    if(state) emit stateChanged(1);
    else emit stateChanged(0);
}

void Engine::readEngine() {
    qint64 b = 1; int c;
    while(getBestmove() == "") {
        char buf[8192];
        memset(buf, 0, sizeof buf);

        //b = engine->read(buf, sizeof(buf));
        b = engine->readLine(buf, sizeof(buf));
        if(b == -1) { cout << "Engine crashed" << endl; return; }
        if(b > 0) {
            buffer.append(buf);
            usleep(100000);
            //cout << buf << " [end]" << endl;
            string line; line.append(buf);
            //cout << line << endl;
            //getValues(QString::fromStdString(line));
            emit newOutput(QString::fromStdString(line));
        }
        //cout << " (" << b << " Bytes read)" << endl;
        if(b <= 0) usleep(10000);
    }
}

string Engine::getBestmove() {
    //cout << "get bestmove: " << bestmove << endl;
    if(bestmove == "") return "";
    return bestmove;
}

void Engine::showOutput(QString outtext) {
    //cout << "show output: "<< outtext.toStdString() << endl;
    output->setText(output->toPlainText().append(outtext));
    output->show();
    output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum());
}

void Engine::setPosition(string fen) {
    cout << "setting position: " << fen << endl;
    this->fen = fen;
}
