#include"uciengine.h"
#include <QtCore>
#include <QtGui>
#include <QWidget>
#include <QTextEdit>
#include <QScrollBar>
#include <QProcess>
#include <boost/lexical_cast.hpp>

UCIEngine::UCIEngine(QObject *parent) : QObject(parent) {

    engine2 = new QProcess();
    engine2->start("./stockfish-6-32.exe");
    engine2->waitForStarted();
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

    /*QThread* engineThread = new QThread;
    EngineReader* reader = new EngineReader(this, engine2, &buffer, &pos);
    reader->moveToThread(engineThread);

    connect(reader, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(engineThread, SIGNAL(started()), reader, SLOT(process()));
    connect(reader, SIGNAL(finished()), engineThread, SLOT(quit()));
    connect(reader, SIGNAL(finished()), reader, SLOT(deleteLater()));
    connect(engineThread, SIGNAL(finished()), engineThread, SLOT(deleteLater()));
    connect(reader, SIGNAL(newOutput()), this, SLOT(getValues()));
    engineThread->start();*/

    /*string message = "isready\n";
    engine2->write(message.c_str(), qstrlen(message.c_str()));*/

    connect(this, SIGNAL(newOutput()), this, SLOT(showOutput()));
    writeToEngine("isready");
    writeToEngine("setoption name MultiPV value 5");
    writeToEngine("setoption name Threads value 4");

    //boost::thread engine ( &EngineThread::readPipe, this );
    output = new QTextEdit();
    output->setReadOnly(true);
}

void UCIEngine::readEngine() {
    qint64 b = 1; int c;
    while(getBestmove() == "") {
        char buf[8192];
        memset(buf, 0, sizeof buf);

        b = engine2->read(buf, sizeof(buf));
        if(b > 0) {
            buffer.append(buf);
            usleep(100000);
            //cout << buf << endl;
            //emit newOutput();
            //parent->getValues();
             //getValues();
        }

        while(buffer.size() > 0 && pos[0] < buffer.size() - 1) {
            //cout << pos[0] << " " << buffer.size() - 1 << endl;
            getValues();
        }

        //cout << " " << b << endl;
        //cout << " (" << b << " Bytes read)" << endl;
        if(b <= 0) usleep(10000);
    }
    //emit finished();
}

void UCIEngine::setThinking(bool state) {
    thinking = state;
    if(state) emit stateChanged(1);
    else emit stateChanged(0);
}

void UCIEngine::setPosition(string fen) {
    cout << "setting position: " << fen << endl;
    this->fen = fen;
}

EngineReader::EngineReader(UCIEngine* parent, QProcess* engine, string* buffer, vector<size_t>* pos) {
    this->engine = engine;
    this->buffer = buffer;
    this->parent = parent;
    this->pos = pos;
}

void EngineReader::process() {
    qint64 b = 1; int c;
    while(1) {
        char buf[8192];
        memset(buf, 0, sizeof buf);

        b = engine->read(buf, sizeof(buf));
        if(b > 0) {
            buffer->append(buf);
            usleep(100000);
            //cout << buf << endl;
            //emit newOutput();
            //parent->getValues();
        }

        /*while(buffer->size() > 0 && pos->at(0) < buffer->size() - 1) {
            cout << pos->at(0) << " " << buffer->size() - 1 << endl;
            emit newOutput();
        }*/

        //cout << " " << b << endl;
        //cout << " (" << b << " Bytes read)" << endl;
        if(b <= 0) usleep(10000);
    }
    emit finished();
}

bool UCIEngine::isThinking() {
    return thinking;
}

bool UCIEngine::stockfish() {
    return true;
}

void UCIEngine::readPipe() {

}

int UCIEngine::writeToEngine(string message) {
    /* Clear MultiPVs from last move  */
    size_t n = 0;
    while(n != string::npos) {
        if(message.substr(n + 1,2) == "go") {
            multipvs.clear();
            cout << "multipvs cleared" << endl;
        }
        n = message.find('\n', n + 1);
    }
    return engine2->write((message + "\n").c_str(), qstrlen((message + "\n").c_str()));

    /*buffer.clear();
    for(int i = 0; i < pos.size(); i++)
    pos[i] = 0;*/
    int b;
    #define	PARENT_READ	readpipe[0]
    #define	CHILD_WRITE	readpipe[1]
    #define CHILD_READ	writepipe[0]
    #define PARENT_WRITE	writepipe[1]
    /*char str[] = "go infinite\n";
    cout << strlen(str) << endl;
    b = write(PARENT_WRITE, str, strlen(str));*/
    //cout << strlen(message.c_str()) << endl;
    message.append("\n");
    b = write(PARENT_WRITE, message.c_str(), strlen(message.c_str()));
    //cout << "writecode: " << b << endl;
    return b;
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

string UCIEngine::getBestmove() {
    if(bestmove == "") return "";
    return bestmove;
}

vector<vector<string>> UCIEngine::getOtherMoves() {
    vector<vector<string>> scoredMultiPVs;
    for(int i = 0; i < multipvs.size(); i++) {
        vector<string> scoredMultiPV;
        scoredMultiPV.push_back(multipvs[i]);
        scoredMultiPV.push_back(boost::lexical_cast<string>(scores[i]));
        //multipvs[i] + " (" + boost::lexical_cast<string>(scores[i]) + ")"
        scoredMultiPVs.push_back(scoredMultiPV);
        cout << i << endl;
    }
    sort(scoredMultiPVs.begin(), scoredMultiPVs.end(), isBigger);
    return scoredMultiPVs;
}

bool UCIEngine::isBigger (vector<string> m,vector<string> n) {
    return (boost::lexical_cast<int>(m[1]) > boost::lexical_cast<int>(n[1]));
}

void UCIEngine::printBestmove() {
    cout << "bestmove: " << bestmove << endl;
}

void UCIEngine::getValues() {
    string outstr = readFromEngine(0);
    emit newOutput();
    while(outstr.size() > 0) {
        //emit newOutput();
        //showOutput();
        if (outstr.size() < 0) return;
        //cout << outstr.size() << " ";

        /* get bestmove */
        int p = outstr.find("bestmove ");
        int e = outstr.find(" ", p + 9);
        if(p != string::npos) {
            bestmove = outstr.substr(p + 9, e- p - 9);
            emit newBestmove();
            setThinking(false);
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
                cout << outstr;
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

                /* Create a new multipv-move */
                if(newMultiPv) {
                    size_t nl = outstr.substr(pv + 4).find("\n");
                    string str = outstr.substr(pv + 4, nl);
                    if(nl != string::npos) {
                        if(str.size() > 4)
                            multipvs.push_back(outstr.substr(pv + 4, outstr.substr(pv + 4).size() - 1));
                        else
                            multipvs.push_back(outstr.substr(pv + 4, nl));
                        scores.push_back(score);
                    } else {
                        cout << "multipv: no newline" << endl;
                    }
                }
                //cout << outstr;
                string output = ""; string html = "";
                for(int i = 0; i < multipvs.size(); i++) {
                    output += multipvs[i];
                    html += "<a href=\"" + multipvs[i] + "\">" + multipvs[i] + "</a><br/>";
                    if(multipvs[i].length() > searchdepth * 4) {
                        //cout << multipvs[i] << endl;
                    }
                }
                //engineOutput->setText(QString::fromStdString(output));
                //engineView->setHtml(QString::fromStdString(html));
                //engineView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
            } else {
                currnr = 0;
                if(toggle) { /*engineOutput->clear();*/ toggle = false; } else toggle = true;
            }
            //cout << currnr << endl;
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
    outstr = readFromEngine(0);
    }
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

    /*command = "position fen " + game[activeBoard]->board->getFenstring();*/
    cout << command << endl;
    writeToEngine(command);
    setThinking(true);
    stopping = false;
    boost::thread engine ( &UCIEngine::readEngine, this );
}

void UCIEngine::stop() {
    if(!stopping) {
        cout << "stopping engine " << endl;
        writeToEngine("stop");
        cout << "engine stopped" << endl;
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
