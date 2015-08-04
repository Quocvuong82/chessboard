#include"enginethread.h"
#include <QtCore>
#include <QtGui>
#include <QWidget>
#include <QTextEdit>
#include <QScrollBar>
#include <boost/lexical_cast.hpp>

EngineThread::EngineThread(QObject *parent) : QThread(parent) {
    boost::thread engine ( &EngineThread::readPipe, this );
    output = new QTextEdit();
    output->setReadOnly(true);
    writeToEngine("setoption name Threads value 4");
    writeToEngine("setoption name MultiPV value 5");
}

void EngineThread::emitMySignal() {
   emit newOutput();
}

bool EngineThread::stockfish() {
    pid_t	childpid;
    pos.push_back(0); pos.push_back(0);
    /*------------------------------------------------------------------------
     * CREATE THE PAIR OF PIPES
     *
     * Pipes have two ends but just one direction: to get a two-way
     * conversation you need two pipes. It's an error if we cannot make
     * them both, and we define these macros for easy reference.
     */
    writepipe[0] = -1;

    if ( pipe(readpipe) < 0  ||  pipe(writepipe) < 0 )
    {
        /* FATAL: cannot create pipe */
        /* close readpipe[0] & [1] if necessary */
    }

    #define	PARENT_READ	readpipe[0]
    #define	CHILD_WRITE	readpipe[1]
    #define CHILD_READ	writepipe[0]
    #define PARENT_WRITE	writepipe[1]

    if ( (childpid = fork()) < 0)
    {
        /* FATAL: cannot fork child */
    }
    else if ( childpid == 0 )	/* in the child */
    {
        ::close(PARENT_WRITE);
        ::close(PARENT_READ);

        dup2(CHILD_READ,  0);  ::close(CHILD_READ); // Redirect stdin
        dup2(CHILD_WRITE, 1);  ::close(CHILD_WRITE); // Redirect stdout

        /* do child stuff */
        char msg[128];

        //cout << read(0,msg, 10);
        /*cout << b << " Bytes read" << endl;
        cout << msg;*/
        fcntl(CHILD_WRITE, F_SETPIPE_SZ, 1048576);
        execl("/bin/stockfish", (char*) "", NULL);
    }
    else				/* in the parent */
    {
        ::close(CHILD_READ);
        ::close(CHILD_WRITE);

        /* do parent stuff */
        connect(this, SIGNAL(newOutput()), this, SLOT(showOutput()));
        output->setWindowTitle("stockfish 6");
    }
    return true;
}

void EngineThread::readPipe() {
    //setbuf(stdout, NULL);
    //setbuf(stdin, NULL);



    //read(PARENT_READ, buf, 20);
    //printf("Received: %s\n", buf);

    int flags = fcntl(PARENT_READ, F_GETFL, 0);
    fcntl(PARENT_READ, F_SETFL, flags | O_NONBLOCK);
    int b = 1; int c;
    while(1) {
        char buf[8192];
        memset(buf, 0, sizeof buf);
        b = read(PARENT_READ, buf, sizeof buf);
        if(b > 0) {
            //cout << buf << " | ";
            /*for(int i = 0; i < b; i++) {
                buffer.append(static_cast<string>(&buf[i]));
            }*/
            buffer.append(buf);
            //buffer.erase(buffer.size()-1);
            usleep(100000);
            size_t newline;
            //newline = buffer.find("\n");
            //cout << buf;
        }
        while(buffer.size() > 0 && pos[0] < buffer.size() - 1) {
            cout << pos[0] << " " << buffer.size() - 1 << endl;
            getValues();
        }

        //cout << " " << b << endl;
        //cout << " (" << b << " Bytes read)" << endl;
        if(b <= 0) usleep(10000);
    }
}

int EngineThread::writeToEngine(string message) {
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
string EngineThread::readFromEngine(int ReaderID) {
    cout << "read from engine (" << ReaderID << endl;
    if (pos[ReaderID] > buffer.size()) return "";
    size_t newline = buffer.find("\n", pos[ReaderID]);
    //cout << "newline: " << newline << " pos: " << pos << endl;
    string s = buffer.substr(pos[ReaderID], newline - pos[ReaderID] + 1);
    if(newline != string::npos) pos[ReaderID] = newline + 1;
    //cout << pos - (newline + 1) << " ";
    //pos = buffer.size();
    return s;
}

string EngineThread::getBestmove() {
    cout << "bestmove: " << bestmove << endl;
    return bestmove;
}

void EngineThread::printBestmove() {
    cout << "bestmove: " << bestmove << endl;
}

void EngineThread::getValues() {
    string outstr = readFromEngine(0);
    while(outstr.size() > 0) {
        emit newOutput();
        if (outstr.size() < 0) return;
        //cout << outstr.size() << " ";

        /* get bestmove */
        int p = outstr.find("bestmove ");
        int e = outstr.find(" ", p + 9);
        if(p != string::npos) {
            bestmove = outstr.substr(p + 9, e- p - 9);
            emit newBestmove();
        }

        /* Collect MultiPV-Data (get possible moves) */
        size_t pv = outstr.find(" pv ");
        size_t multipv = outstr.find(" multipv ");
        if(pv != string::npos && multipv != string::npos) {
            size_t sp = outstr.find(" ", multipv + 10);
            string multipvstr = outstr.substr(multipv + 9, sp - (multipv + 9));
            //multipvstr.erase(multipvstr.size() - 1);
            //cout << multipvstr << endl;
            bool toggle = false;
            if(boost::lexical_cast<int>(multipvstr) > currnr) {
                //currmoves.push_back(outstr.substr(pv + 4));
                currnr++;
                bool newMultiPv = true;
                for(int i = 0; i < multipvs.size(); i++) {
                    //cout << multipvs[i] << " == " << outstr.substr(pv + 4, multipvs[i].size()) << endl;
                    if(multipvs[i] == outstr.substr(pv + 4, multipvs[i].size())) {
                        multipvs[i] = outstr.substr(pv + 4, outstr.substr(pv + 4).size() - 1);
                        newMultiPv = false;
                    }
                }
                if(newMultiPv) {
                    size_t nl = outstr.substr(pv + 4).find("\n");
                    string str = outstr.substr(pv + 4, nl);
                    if(nl != string::npos) {
                        if(str.size() > 4)
                            multipvs.push_back(outstr.substr(pv + 4, outstr.substr(pv + 4).size() - 1));
                        else
                            multipvs.push_back(outstr.substr(pv + 4, nl));
                    } else {
                        cout << "no newline" << endl;
                    }
                }
                //cout << outstr;
                string output = ""; string html = "";
                for(int i = 0; i < multipvs.size(); i++) {
                    output += multipvs[i];
                    html += "<a href=\"" + multipvs[i] + "\">" + multipvs[i] + "</a><br/>";
                    if(multipvs[i].length() > 5 * 4) {
                        writeToEngine("stop");
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
vector<string> EngineThread::getMultiPV() {
    return multipvs;
}

void EngineThread::showOutput() {
    output->setText(output->toPlainText().append(QString::fromStdString(readFromEngine(1))));
    output->show();
    output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum());
}
