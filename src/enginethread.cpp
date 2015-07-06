#include"enginethread.h"
#include <QtCore>
#include <QtGui>
#include <QWidget>

EngineThread::EngineThread(QObject *parent) : QThread(parent) {
    boost::thread engine ( &EngineThread::readPipe, this );
}

void EngineThread::emitMySignal() {
   emit newOutput();
}

bool EngineThread::stockfish() {
    pos = 0;
    pid_t	childpid;

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

        execl("/bin/stockfish", (char*) "", NULL);
    }
    else				/* in the parent */
    {
        ::close(CHILD_READ);
        ::close(CHILD_WRITE);

        /* do parent stuff */
    }
    return true;
}

void EngineThread::readPipe() {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);



    //read(PARENT_READ, buf, 20);
    //printf("Received: %s\n", buf);

    int flags = fcntl(PARENT_READ, F_GETFL, 0);
    fcntl(PARENT_READ, F_SETFL, flags | O_NONBLOCK);
    int b = 1; int c;
    while(1) {
        char buf[64];
        memset(buf, 0, sizeof buf);
        b = read(PARENT_READ, buf, 64);
        if(b > 0) {
            //cout << buf << " | ";
            buffer.append(buf);
            usleep(1000);
            size_t newline;
            newline = buffer.find("\n");
            if(newline != string::npos) emit newOutput();
        }
        //cout << " (" << b << " Bytes read)" << endl;
        if(b <= 0) usleep(10000);
    }
}

int EngineThread::writeToEngine(string message) {
    buffer.clear();
    pos = 0;
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
string EngineThread::readFromEngine() {
    if (pos > buffer.size()) return "";
    size_t newline = buffer.find("\n", pos);
    //cout << "newline: " << newline << " pos: " << pos << endl;
    string s = buffer.substr(pos, newline - pos + 1);
    if(newline != string::npos) pos = newline + 1;
    //pos = buffer.size();
    return s;
}
