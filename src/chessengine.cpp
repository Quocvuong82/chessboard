#include "chessengine.h"
#include <iostream>
#include <boost/thread.hpp>

using namespace std;

ChessEngine::ChessEngine(QString pathToEngine) : Engine(pathToEngine)
{
    connect(this, SIGNAL(newOutput(QString)), this, SLOT(getValues(QString)));
}

void ChessEngine::go() {
    buffer = "";
    output->clear();
    bestmove = "";

    //writeToEngine("stop");

    /* Create Command with Fen-String */
    string command;

    //command = "setboard fen " + fen + "\n";

    command += "go";

    /* Add Search Parameters */
    /*if(searchdepth < 0 && movetime < 0) command += " infinite";
    else {
        if (searchdepth > 0) command += " depth " + boost::lexical_cast<string>(searchdepth);
        if(movetime > 0) command += " movetime " + boost::lexical_cast<string>(movetime);
        if(mate > 0) command += " movetime " + boost::lexical_cast<string>(mate);
        if(nodes > 0) command += " movetime " + boost::lexical_cast<string>(nodes);
    }*/

    /*command = "position fen " + game[activeBoard]->board->getFenstring();*/
    cout << command << endl;
    writeToEngine(command);
    setThinking(true);
    //stopping = false;
    boost::thread engine ( &ChessEngine::readEngine, this );
}

void ChessEngine::setPosition(string fen) {
    cout << "setting position: " << fen << endl;
    this->fen = fen;
}

vector<vector<string>> ChessEngine::getOtherMoves() {
    vector<vector<string>> empty;
    return empty;
}

void ChessEngine::stop() {

}

void ChessEngine::getValues(QString line) {
    cout << "get values " << endl;
    string outstr = line.toStdString();
    //while(outstr.size() > 0) {
        if (outstr.size() < 0) return;

        /* get bestmove */
        string needle = "\nmove ";
        int p = outstr.find(needle);
        int e = outstr.find('\n', p + needle.length());
        if(p != string::npos) {
            bestmove = outstr.substr(p + needle.length(), 4);
            cout << "Engine::getValues: bestmove " << bestmove << endl;
            emit newBestmove();
            setThinking(false);
        }
    //}
}
