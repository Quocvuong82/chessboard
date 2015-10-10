#include "chessengine.h"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

ChessEngine::ChessEngine(QString pathToEngine) : Engine(pathToEngine)
{
    connect(this, SIGNAL(newOutput(QString)), this, SLOT(getValues(QString)));
    writeToEngine("st 45");
}

void ChessEngine::go() {
    buffer = "";
    output->clear();
    bestmove = "";

    //writeToEngine("stop");

    /* Create Command with Fen-String */
    string command;

    cout << "set fen: " << fen << endl;
    command = "setboard " + fen + "\n";

    /* Add Search Parameters */
    //if(searchdepth < 0 && movetime < 0) command += " infinite";
    //else {
        if (searchdepth > 0) command += " sd " + boost::lexical_cast<string>(searchdepth) + "\n";
        if(movetime > 0) command += " st " + boost::lexical_cast<string>(movetime) + "\n";
        //if(mate > 0) command += " movetime " + boost::lexical_cast<string>(mate);
        //if(nodes > 0) command += " movetime " + boost::lexical_cast<string>(nodes);
    //}

    command += "go";


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
    writeToEngine("force");
}

void ChessEngine::getValues(QString line) {
    cout << "get values " << endl;
    string outstr = line.toStdString();
    //while(outstr.size() > 0) {
        if (outstr.size() < 0) return;

        /* get bestmove */
        string needle = "move ";
        int p = outstr.find(needle);
        int e = outstr.find('\n', p + needle.length());
        if(p != string::npos) {
            if(outstr.substr(p + needle.length(), 4) != "eval") {
                bestmove = outstr.substr(p + needle.length(), 4);
                cout << "ChessEngine::getValues: bestmove " << bestmove << endl;
                emit newBestmove();
                setThinking(false);
            }
        }

        /* get depth */
        needle = " d: ";
        p = outstr.find(needle);
        e = outstr.find(' ', p + needle.length());
        if(p != string::npos) {
            cout << "depth: " << outstr.substr(p + needle.length(), e - p - needle.length()) << endl;
            int depth = boost::lexical_cast<int>(outstr.substr(p + needle.length(), e - p - needle.length()));
            if(depth > this->depth) {
                this->depth = depth;
                //multipvs.clear(); scores.clear(); // Clear Multi-PV Moves
                emit newDepth(depth);
            }
        }
    //}
}

void ChessEngine::setSearchDepth(int value) {
    searchdepth = value;
}
void ChessEngine::setMovetime(int value) {}
void ChessEngine::setNodes(int value) {}
