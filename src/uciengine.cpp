#include"uciengine.h"
#include <QtCore>
#include <QtGui>
#include <QWidget>
#include <QTextEdit>
#include <QScrollBar>
#include <QProcess>
#include <boost/lexical_cast.hpp>

UCIEngine::UCIEngine(QString pathToEngine) : Engine(pathToEngine) {

    buffer = "";
    for(int i = 0; i < 2; i++) {
        pos.push_back(0);
    }
    setThinking(false);
    bestmove = "";
    searchdepth = 5;
    movetime = -1;
    mate = -1;
    nodes = -1;

    //disconnect(this, SIGNAL(newOutput(QString)));
    //connect(this, SIGNAL(newOutput(QString)), this, SLOT(showOutput(QString)));
    connect(this, SIGNAL(newOutput(QString)), this, SLOT(getValues(QString)));

    //connect(this, SIGNAL(newOutput()), this, SLOT(showOutput()));
    //writeToEngine("go");
    writeToEngine("setoption name MultiPV value 5");
    writeToEngine("setoption name Threads value 4");

    output->setParent(0);
    /*output = new QTextEdit();
    output->setReadOnly(true);*/
}

int UCIEngine::writeToEngine(string message) {
    /* Clear MultiPVs from last move  */
    size_t n = 0;
    while(n != string::npos) {
        if(message.substr(n + 1,2) == "go") {
            multipvs.clear();
            //qDebug() << "UCIEngine:" << "multipvs cleared" << endl;
        }
        n = message.find('\n', n + 1);
    }
    return Engine::writeToEngine(message);
}

/* Returns Engine Output Line By Line */
string UCIEngine::readFromEngine(int ReaderID) {
    //cout << "read from engine (" << ReaderID << ")" << endl;
    if (pos[ReaderID] > buffer.size()) return "";
    size_t newline = buffer.find("\n", pos[ReaderID]);
    //cout << "newline: " << newline << " pos: " << pos[ReaderID] << endl;
    string s = buffer.substr(pos[ReaderID], newline - pos[ReaderID] + 1);
    if(newline != string::npos) pos[ReaderID] = newline + 1;
    //cout << pos - (newline + 1) << " ";
    //pos = buffer.size();
    return s;
}

vector<string> UCIEngine::getUniqueMultiPV(int depth) {
    vector<string> uniqueMultiPV;
    for(int i = multipvs.size() - 1; i >= 0; i--) {
        bool unique = true;
        for(int j = 0; j < uniqueMultiPV.size(); j++) {
            if(uniqueMultiPV[j] == multipvs[i].substr(0, depth * 4)) unique = false; // we have this move already
        }
        if(unique) {
            uniqueMultiPV.push_back(multipvs[i].substr(0, depth * 4));
            uniqueScores.push_back(scores[i]); // Get the score in centi-pawns of the move
        }
    }
    return uniqueMultiPV;
}

vector<vector<string>> UCIEngine::getOtherMoves() {
    qDebug() << "UCIEngine:" << "get other moves";
    qDebug() << "UCIEngine:" << "multivs.size " << multipvs.size();
    vector<vector<string>> scoredMultiPVs;
    for(int i = 0; i < multipvs.size(); i++) {
        vector<string> scoredMultiPV;
        scoredMultiPV.push_back(multipvs[i]);
        scoredMultiPV.push_back(boost::lexical_cast<string>(scores[i]));
        //multipvs[i] + " (" + boost::lexical_cast<string>(scores[i]) + ")"
        scoredMultiPVs.push_back(scoredMultiPV);
    }
    sort(scoredMultiPVs.begin(), scoredMultiPVs.end(), isBigger);
    return scoredMultiPVs;
}

bool UCIEngine::isBigger (vector<string> m,vector<string> n) {
    return (boost::lexical_cast<int>(m[1]) > boost::lexical_cast<int>(n[1]));
}

void UCIEngine::getValues(QString line) {
    //qDebug() << "UCIEngine::getValues";

    if(line.size() == 0) return;
    string outstr = line.toStdString(); // = str.substr(oldline, newline - oldline);

    /* get bestmove */
    int p = outstr.find("bestmove ");
    int e = outstr.find(" ", p + 9);
    if(p != string::npos) {
        bestmove = outstr.substr(p + 9, e- p - 9);
        qDebug() << "UCIEngine: bestmove" << QString::fromStdString(bestmove);
        qDebug() << "UCIEngine:" << "set thinking false";
        setThinking(false);
        emit newBestmove();
        return;
    }

    /* get depth */
    p = outstr.find("depth ");
    e = outstr.find(" ", p + 6);
    if(p != string::npos) {
        int depth = boost::lexical_cast<int>(outstr.substr(p + 6, e- p - 6));
        if(depth > this->depth) {
            this->depth = depth;
            multipvs.clear(); scores.clear(); // Clear Multi-PV Moves
            emit newDepth(depth);
        }
    }

    /* Collect MultiPV-Data (get possible moves) */
    size_t pv = outstr.find(" pv ");
    size_t multipv = outstr.find(" multipv ");
    if(pv != string::npos && multipv != string::npos) {
        size_t sp = outstr.find(" ", multipv + 10);
        string multipvstr = outstr.substr(multipv + 9, sp - (multipv + 9));
        bool toggle = false;
        if(boost::lexical_cast<int>(multipvstr) > currnr) {
            currnr++;

            /* Get the score of the MulitPV-Move */
            size_t sc = outstr.find(" score ");
            size_t sp = outstr.find(" ", sc + 10);
            string score_str = outstr.substr(sc + 10, sp - sc - 10);
            //cout << outstr;
            //cout << "multipv: " << m << "multipv-score: " << sc << " " << sp << " " << score_str << endl;
            int score;
            if(outstr.substr(sc + 7, 4) == "mate")
                score = numeric_limits<int>::max();
            else
                score = boost::lexical_cast<int>(score_str);

            /* Check if we have a multipv-move to update */
            bool newMultiPv = true;
            for(int i = 0; i < multipvs.size(); i++) {
                //cout << multipvs[i] << " == " << outstr.substr(pv + 4, multipvs[i].size()) << endl;
                if(multipvs[i] == outstr.substr(pv + 4, multipvs[i].size())) {
                    multipvs[i] = outstr.substr(pv + 4, outstr.substr(pv + 4).size() - 1);
                    scores[i] = score;
                    newMultiPv = false;
                }
            }
            //qDebug() << QString::fromStdString(outstr);

            /* Create a new multipv-move */
            if(newMultiPv) {
                //qDebug() << "UCIEngine:" << "creating new multipv";
                size_t lineEnd = outstr.size();
                string str = outstr.substr(pv + 4, lineEnd);
                if(lineEnd != string::npos) {
                    if(str.size() > 4)
                        multipvs.push_back(outstr.substr(pv + 4, outstr.substr(pv + 4).size() - 1));
                    else
                        multipvs.push_back(outstr.substr(pv + 4, lineEnd));
                    scores.push_back(score);
                } else {
                    //qDebug() << "UCIEngine:" << "multipv: no newline";
                }
            }
        } else {
            currnr = 0;
            if(toggle) toggle = false; else toggle = true;
        }
    }

    /* get score of the best move */
    /*size_t sc = 0;
    sc = outstr.find("score ");
    while(sc < outstr.length()) {
        pv = outstr.find(" pv ", sc + 1);
        int l = 1;
        if(sc < outstr.length()) {
            //cout << outstr.length() << " " << sc << " " << pv << endl;
            if((sc + 9) < outstr.length()) {
                //string str = outstr.substr(sc + 10);
                l = outstr.substr(sc + 9).find(" ");
            }
            //cout << "score: ";
            if((sc + 9 + l) < outstr.length()) {
                //cout << outstr.substr(sc + 9, l) << endl;
                scores.push_back(outstr.substr(sc + 9, l));
                if(pv != string::npos) moves.push_back(outstr.substr(pv + 4,outstr.substr(pv + 4).size() - 1));
            }
        }
        sc = outstr.find("score ", sc + 1);
    }*/
    //qDebug() << "UCIEngine:" << "multipvs.size = " << multipvs.size();
    //qDebug() << "UCIEngine:" << "getting values finished";
}

vector<string> UCIEngine::getMultiPV() {
    return multipvs;
}

void UCIEngine::showOutput() {
    string outtext;
    while(pos[1] < buffer.size() - 1) {
        outtext.append(readFromEngine(1));
    }

    output->setText(output->toPlainText().append(QString::fromStdString(outtext)));
    output->show();
    output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum());
}

void UCIEngine::go() {
    cout << "UCIEngine::go" << endl;
    buffer = "";
    for(int i = 0; i < 2; i++) {
        pos[i] = 0;
    }
    output->clear();
    bestmove = "";
    multipvs.clear(); scores.clear();
    depth = 0;

    writeToEngine("stop");

    /* Create Command with Fen-String */
    string command;

    command = "position fen " + fen + "\n";

    command += "go";

    /* Add Search Parameters */
    if(searchdepth < 0 && movetime < 0) command += " infinite";
    else {
        if (searchdepth > 0) command += " depth " + boost::lexical_cast<string>(searchdepth);
        if(movetime > 0) command += " movetime " + boost::lexical_cast<string>(movetime);
        if(mate > 0) command += " movetime " + boost::lexical_cast<string>(mate);
        if(nodes > 0) command += " movetime " + boost::lexical_cast<string>(nodes);
    }

    //qDebug() << "UCIEngine:" << command << endl;
    writeToEngine(command);
    qDebug() << "UCIEngine:" << "set thinking true";
    setThinking(true);
    stopping = false;
    boost::thread engine ( &UCIEngine::readEngine, this );
}

void UCIEngine::stop() {
    qDebug() << "UCIEngine::stop()" << endl;
    if(!stopping) {
        qDebug() << "UCIEngine:" << "stopping engine " << endl;
        writeToEngine("stop");
        qDebug() << "UCIEngine:" << "engine stopped" << endl;
    }
    stopping = true;
    setThinking(false);
}

void UCIEngine::setSearchDepth(int value) {
    //cout << "setting search depth " << value << endl;
    searchdepth = value;
}

void UCIEngine::setMovetime(int value) {
    //cout << "setting movetime " << value << endl;
    movetime = value;
}

void UCIEngine::setMate(int value) {
    //cout << "setting mate " << value << endl;
    mate = value;
}
void UCIEngine::setNodes(int value) {
    cout << "setting nodes " << value << endl;
    nodes = value;
}
