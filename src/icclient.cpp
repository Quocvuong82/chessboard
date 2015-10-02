#include"icclient.h"
#include <QScrollBar>
#include <QLineEdit>

ICclient::ICclient(QObject *parent) : QObject(parent){
    host = "freechess.org";
    portno = 5000;
}

/*ICclient::ICclient(string hostname, int portno) : QObject(parent){
    host = hostname;
    this->portno = portno;
}*/

bool ICclient::connect() {
    AUTOLOAD = true;
    pos = 0;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    /*if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }*/
    //portno = 5000;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(host.c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    cout << "Connecting with " << host << "..." << endl;

    connected = true;

    browser = new QWidget();
    output = new QTextEdit();
    QVBoxLayout* browserLayout = new QVBoxLayout();
    input = new QLineEdit();
    browserLayout->addWidget(output);
    browserLayout->addWidget(input);
    browser->setLayout(browserLayout);
    browser->resize(571, 321);
    browser->setWindowTitle("Internet Chess Server - Chessboard");
    browser->show();

    QObject::connect(input, SIGNAL(returnPressed()), this, SLOT(readInput()));
    QObject::connect(this, SIGNAL(newLine(QString)), this, SLOT(printOutput(QString)));

    // Print continuously incoming messages from the server
    boost::thread outputThread ( &ICclient::readSocket, this );
    //boost::thread bufferManager ( &ICclient::bufferManager, this );

    // Entering Username
    cout << "Entering username " << endl;
    writeSocket("guest\n");

    // Entering Password
    cout << "Entering password " << endl;
    writeSocket("\n");

    cout << "Successfully connected with FICS guest account" << endl;

    usleep(999);
    writeSocket("\n");
    // Set style-variable to 12
    cout << "setting style variable to 12 " << endl;
    writeSocket("style 12\n");

    cout << "set formula to no-match" << endl;
    writeSocket("set formula standard=1 & blitz=1\n");
    //fcntl(sockfd, F_SETFL, O_NONBLOCK);
    // Read welcome message from server
}

void ICclient::emitSignal(){
    emit newOutput();
}

string ICclient::readFromServer(){

    /* Returns Server Outout Line By Line */
    if (pos > buffer.size() || pos == string::npos) return "";
    size_t newline = buffer.find("\n", pos);
    string s = buffer.substr(pos, newline - pos + 1);
    if(newline != string::npos) pos = newline + 1; else
    pos = buffer.size() + 1;
    size_t n = s.find("\n");
    while(n != string::npos) {
        n = s.find("\n", n + 1);
    }
    s.erase(0, 1); // remove newline character at the beginning
    return s;
}

bool ICclient::isunread() {
    if (buffer.size() > pos) return true;
    else return false;
}

bool ICclient::isunread2() {
    if (buffer.size() > Rpos) return true;
    else return false;
}

string ICclient::readLine(){
    /* Returns Server Outout Line By Line */
    //cout << "Rpos: " << Rpos << " pos: " << pos << endl;
    if (Rpos > buffer.size()) return "";
    size_t newline = string::npos;
    newline = buffer.find("\n", Rpos);
    /*while(newline == string::npos) {
        newline = buffer.find("\n", Rpos);
        usleep(1000);
    }*/
    if(newline == string::npos) return "";

    string s = buffer.substr(Rpos, newline - Rpos + 1);
    if(newline != string::npos) Rpos = newline + 1;
    size_t n = 0;
    while(n != string::npos) {
        //cout << n << " ";
        n = s.find("\n", n + 1);
    }
    //cout << endl;
    if(s[0] == '\n') s.erase(0, 1); // remove newline character at the beginning
    return s;
}

void ICclient::error(const char *msg)
{
    perror(msg);
    exit(0);
}

string ICclient::readSocket() {
    /*int n;
    char buffer[32];
    n = read(sockfd,buffer,32);

    if (n < 0) {
        error("ERROR reading from socket");
    }
    string answer(buffer);
    bzero(buffer,32);

    if(buffer[0] == '\n') emit newOutput();
    return answer.substr(0, answer.size() - 6);*/
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);



    //read(PARENT_READ, buf, 20);
    //printf("Received: %s\n", buf);

    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    int b = 1; int c;
    size_t newline; size_t newline_old = 0;
    while(AUTOLOAD) {
        char buf[68];
        memset(buf, 0, sizeof buf);
        b = read(sockfd, buf, sizeof buf);
        if(b > 0) {
            //cout << buf << " | ";
            size_t bsize = buffer.size();
            buffer.append(buf);
            usleep(50);
            newline = buffer.find("\n", bsize);
            if(newline != string::npos) {
                //emit newOutput();
                emit newLine(QString::fromStdString(buffer.substr(newline_old + 2, newline - newline_old - 1)));
                newServerData = true;
                newline_old = newline;
            }
            if(latency > 501) latency -= 500; // make reading faster
        }
        //cout << " (" << b << " Bytes read)" << endl;
        if(b <= 0) {
            if(latency < 100000) latency += 500; // slow down reading
            usleep(latency);
        }
        //cout << latency << endl;
    }
    while(!AUTOLOAD);
    readSocket();
    return string();
}

bool ICclient::writeSocket(string message) {
    //cout << "wS: " << message << endl;
    regex expr("^(savegame)[ ][A-z]+[ ][0-9]+");
    regex expr2("^(savegames)[ ][A-z]+");
    regex expr3("^(scan fics)");
    smatch m;
    if(regex_search(message, m, expr)) {
        size_t len; // length of handle
        len = message.substr(9).find(' ');
        string handle = message.substr(9, len);
        int gameID = boost::lexical_cast<int>(message.substr(10+len, message.length()));
        /*cout << "handle: " << handle << endl;
        cout << "gameID: " << gameID << endl;*/
        saveGameToDB(handle, gameID);
        cout << "finished saving to DB" << endl;
    } else if (regex_search(message, m, expr2)) {
        size_t len; // length of handle
        len = message.substr(10).find(' ');
        string handle = message.substr(10, len);
        boost::thread gamesaver(&ICclient::saveGames, this, handle);
        //saveGames(handle);
    } else if (regex_search(message, m, expr3)) {
        boost::thread gamesaver(&ICclient::scanFics, this);
    } else {
        message.append("\n");
        int n;
        char buffer[256];
        bzero(buffer,256);
        std::strcpy (buffer, message.c_str());
        n = write(sockfd,buffer,strlen(buffer));
        //cout << "n: " << n << endl;
        if (n < 0) {
             error("ERROR writing to socket");
            return false;
        }
    }
    return true;
}

void ICclient::scanFics() {
    myChessDB = ChessDatabase("localhost", "root", "floppy", "schach");
    vector<string> handle = myChessDB.getPlayersToScan();
    for(int i = 0; i < handle.size(); i++) {
        saveGames(handle[i]);
    }
}

int ICclient::saveGames(string handle) {
    cout << "savegames " << handle << endl;
    string answer;

    if(pos != string::npos) {
        Rpos = pos; // set buffer read position
        pos = string::npos; // set pos for main output to -1 --> there will be no output
    }

    writeSocket("history " + handle + "\n");
    answer = readLine();
    regex expr("[0-9 ][0-9][:][ ][=\+\-]");
    smatch m;

    int attempt = 0;
    while(!regex_search(answer, m, expr) && attempt < 10000) {
        answer.clear();
        answer = readLine();
        attempt++;
        usleep(1000);
    }
    if(!regex_search(answer, m, expr)) { cout << "no such handle " << handle << endl; return 0; }

    cout << answer << endl;
    string gameIDstr = m.str().substr(0,2);
    if(gameIDstr[0] == ' ') gameIDstr.erase(0,1);
    int gameID = boost::lexical_cast<int>(gameIDstr);
    //cout << gameID << " bis " << gameID + 9 << endl;
    for(int i = 0; i < 10; i++) {
        //clearBuffer();
        if(gameID + i > 99) gameID = 0 - i;
            saveGameToDB(handle, gameID + i);
    }
    return p;

}

int ICclient::saveGameToDB(string handle, int gameID) {
    cout << "Saving Game " << gameID << " from " << handle << " to Database..." << endl;
    string message; string answer;

    if(pos != string::npos) {
        Rpos = pos; // set buffer read position
        pos = string::npos;
    }

    /* Get Elo */
    usleep(2000);
    cout << Rpos << " " << buffer.size() << endl;
    Rpos = buffer.size(); // skip previous server output
    message = "history " + handle + "\n";
    writeSocket(message);

    size_t pos = string::npos; //size_t pos2 = 0;

    int attempt = 0; answer.clear();
    while((pos > 3 || pos == string::npos) && attempt < 1000) {
        answer.clear();
        answer = readLine();
        attempt++;
        usleep(1000);
        pos = answer.find(lexical_cast<string>(gameID) + ": ");
        //pos2 = answer.find("goes forward");
    }
    cout << Rpos << " " << buffer.size() << endl;
    cout << answer << endl;
    if(pos == string::npos) { cout << "No such game " << gameID << endl; return 0; }

    cout << "get elo " << endl;
    // Get Elo from history
    int whiteElo; int blackElo; string date;
    //pos = answer.find(lexical_cast<string>(gameID) + ": ");
    if(gameID < 10) pos--; // Correct position for single digit numbers
    //cout << answer << endl;
    if(pos != string::npos) {
        char color = answer[pos + 11]; // User 1's color in the match
        string elo;
        /* Find out which player played which color */
        if(color == 'W') {
            elo = answer.substr(pos + 6, 4);
            trim(elo); // Remove whitespaces in front of smaller numbers "   0" --> "0"
            whiteElo = lexical_cast<int>(elo);
            elo = answer.substr(pos + 13, 4);
            trim(elo);
            blackElo = lexical_cast<int>(elo);
        } else {
            elo = answer.substr(pos + 13, 4);
            trim(elo);
            whiteElo = lexical_cast<int>(elo);
            elo = answer.substr(pos + 6, 4);
            trim(elo);
            blackElo = lexical_cast<int>(elo);
        }
        cout << "white Elo: " << whiteElo << endl;
        cout << "black Elo: " << blackElo << endl;
        cout << pos << endl;
        date = answer.substr(pos+53, 26);
        cout << date << endl;
    }

    message = "examine " + handle + " " + lexical_cast<string>(gameID) + "\n";
    writeSocket(message);

    regex style12(".*(<12> ).*$");
    string position;
    vector<string> fenstrings(13);
    Board* board;
    smatch m;
    int i = 0;
    string white; string black;
    int GameID = 0; int PosID = 1;

    answer.clear();
    newServerData = false;
    answer = readLine();
    while(answer.find("<12>") == string::npos && !newServerData) {
          answer.clear();
          answer = readLine();
          usleep(500);
    }

    /* Skip initial position */
    writeSocket("forward\n");

    answer.clear();
    newServerData = false;
    answer = readLine();
    while(answer.find("<12>") == string::npos && !newServerData) {
          answer.clear();
          answer = readLine();
          usleep(500);
    }
    /*newServerData = false;
    while(!newServerData);
    answer = readLine();*/
    //cout << answer << endl;

    // Scan positions to DataBase
    int new_p = 0; // new positions counter
    do {
        newServerData = false;
        writeSocket("forward\n");
        answer.clear();
        answer = readLine();
        while(answer.find("<12>") == string::npos && answer.find("end of the game") == string::npos && answer.size() < 159) {
            answer.clear();
            answer = readLine();
            //cout << answer << endl;
            usleep(100);
        }
        if(answer.find("end of the game") != string::npos) break;

        newServerData = false;
        //cout << "substr find <12>" << endl;
        //cout << position << endl;
        position = answer.substr(answer.find("<12>"));
        for (int j = 0; j < 8; j++) {
            //cout << "fenstr" << j << endl;
            fenstrings[j] = position.substr((5 + 9 * j), 8);
            //cout << "f"<< j << " " << fenstrings[j] << endl;
        }


        size_t b = 0; size_t e;
        int x = 0;
        e = position.find(" ", b + 1);
        vector<string> values;

        //cout << "get values from style12" << endl;
        //cout << position << endl;
        /* Get values from style12-string */
        while(e != string::npos && x < 50) {
            //cout << position.substr(b + 1,e - (b+1)) << endl;
            values.push_back(position.substr(b + 1,e - (b+1)));
            x++;
            b = e;
            e = position.find(" ", b + 1);
        }
        /*for(int i = 0; i < values.size(); i++) {
            cout << boost::lexical_cast<string>(i) << ": " << values[i] << endl;
        }*/

        //cout << "set active color " << endl;
        fenstrings[8] = values[9][0] + 32; // active color (--> to lower case)
        //cout << "set castling values" << endl;
        if(values[11] == "1") fenstrings[9] = "K"; else fenstrings[9] = "-";
        if(values[12] == "1") fenstrings[9] += "Q"; else fenstrings[9] += "-";
        if(values[13] == "1") fenstrings[9] += "k"; else fenstrings[9] += "-";
        if(values[14] == "1") fenstrings[9] += "q"; else fenstrings[9] += "-";
        //fenstrings[9] += " ";
        if(values[10] == "-1") fenstrings[10] = "-"; else fenstrings[10] = values[10];
        //fenstrings[9] += " ";
        //cout << "set other value " << endl;
        fenstrings[11] = values[15];
        //fenstrings[9] += " ";
        //cout << "set move number" << endl;
        fenstrings[12] = values[26];
        //cout << fenstrings[12] << endl;

        board = new Board(Fen());
        board->setPosition(Fen(fenstrings));
        board->setGameDate(date);
        //board->print();
        size_t offset = 0;
        for(int x = 0; x < 17; x++) {
            offset += position.substr(offset).find(' ');
            offset++;
            //cout << offset << endl;
        }
        white = position.substr(offset, position.substr(offset + 1).find(' ') + 1);
        offset += position.substr(offset).find(' ');
        black = position.substr(offset + 1, position.substr(offset + 1).find(' '));
        if(i == 0) {
            cout << endl;
            cout << "white: " <<  white << " (" << whiteElo << ") ";
            cout << "black: " <<  black << " (" << blackElo << ")" << endl;
        }
        if(white == handle)
            board->setPlayer(1, white, whiteElo, white, true);
        else
            board->setPlayer(1, white, whiteElo, white);
        if(black == handle)
            board->setPlayer(0, black, blackElo, black, true);
        else
            board->setPlayer(0, black, blackElo, black);
        if(i > 0) board->setGameID(GameID);

        board->setParent(PosID);

        // Write to DataBase
        if(board->writePositionToDB()) new_p++;
        //cout << board->getPositionIDFromDB() << endl;
        if(i == 0) {
            board->writePlayersToDB();
            if(!board->writeGameToDB()) break;
            GameID = board->getGameID();
        }
        PosID = board->getPositionIDFromDB();
        board->updateGame(PosID);
        delete board;
        i++;
    } while (answer.find("<12>") != string::npos); // answer.find("end of the game") == string::npos
    p += new_p;
    cout << p << " new Positions have been written to DB" << endl;

    //board[i] = Board(Fen(fenstrings));
    //LOADFILTERED = false;
    //cout << "autoload on" << endl;
    AUTOLOAD = true;
    writeSocket("unexamine\n");
    this->pos = Rpos;
    return new_p;
}

void ICclient::bufferManager() {
    while(connected) {
       while(buffer.size() > pos) {
           //cout << "buffdiff: " << buffer.size() - pos << endl;
           emit unread();
           usleep(5000);
       }
       usleep(50000);
    }
}

void ICclient::clearBuffer() {
    pos = 0; Rpos = 0;
    buffer.clear();
}

bool ICclient::isConnected() {
    return connected;
}

void ICclient::getGame() {
    writeSocket("getgame");
}

void ICclient::printOutput(QString line) {
    outstr += line;
    output->setText(outstr);
    output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum());
}

void ICclient::readInput() {
    string msg = input->text().toStdString();
    size_t o = msg.find("observe ");
    size_t ex = msg.find("examine ");
    //cout << "o: " << o << endl;
    if(o != string::npos) {
        size_t sp = msg.find(" ", o + 8);
        int gameID = boost::lexical_cast<int>(msg.substr(o + 8, sp - o - 8));
        emit newGameID(gameID); // --> active Board can set new game id to identify game data from server
        //BoardTab->setTabText(activeBoard, QString::fromStdString("#" + boost::lexical_cast<string>(gameID)));
        cout << "set gameID to " << msg.substr(o + 8, sp - o - 8) << endl;
    }
    if(ex != string::npos) {
        size_t sp = msg.find(" ", ex + 8);
        size_t sp2 = msg.find(" ", sp + 1);
        sp++;
        int gameID = boost::lexical_cast<int>(msg.substr(sp, sp2));
        //game[activeBoard]->setGameID(gameID); // will not work, cause this is not the real game id (it's the ID from history)
        //BoardTab->setTabText(activeBoard, QString::fromStdString("#" + boost::lexical_cast<string>(gameID)));
        cout << "set gameID to " << msg.substr(sp, sp2) << endl;
    }
    writeSocket(msg);
}
