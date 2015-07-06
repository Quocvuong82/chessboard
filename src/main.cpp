#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "fen.h"
#include "board.h"
#include <fcntl.h>


using namespace std;
using namespace boost;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int sockfd;
static bool AUTOLOAD = true;
static bool LOADFILTERED = true;

string readS(int sockfd) {
	int n;
	char buffer[4096];
	n = read(sockfd,buffer,4096);
	if (n < 0) {
		error("ERROR reading from socket");
	}
	string answer(buffer);
	bzero(buffer,4096);
	return answer;
}
bool writeS(int sockfd, string message) {
	int n;
	char buffer[256];
	bzero(buffer,256);
	std::strcpy (buffer, message.c_str());
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0) {
		 error("ERROR writing to socket");
		return false;
	}
	return true;
}
string host = "freechess.org";

bool history(){
    // Entering Command
	cout << "Enter a handle to scan: ";
	string handle;
	cin >> handle;
	cout << "history " << handle << endl;
	writeS(sockfd, "history " + handle + "\n");

	int j = 0;
	while(j < 999999999) j++;

	// Read answer from Server
    string history(readS(sockfd));

    size_t len = history.length();
    size_t begin = 80; // where the first dataset starts
    size_t datasetlen = 81;
    vector<string> historydata(20);
    int i = 0;
    while (begin < len) {
		cout << history.substr(begin, datasetlen) << endl;
		historydata[i] = history.substr(begin, datasetlen);
		begin += datasetlen;
		i++;
    }
    cout << historydata[5].substr(2, 2) << endl;
    return true;
}

bool examine(string handle) {
	cout << "Enter a game id to examine: ";
	string gameID;
	cin >> gameID;
	//gameID = historydata[5].substr(2, 2);
	cout << "Examining Game " << gameID << endl;
	string message = "examine " + handle + " " + gameID + "\n";
	cout << message << endl;
	writeS(sockfd, message);
	cout << readS(sockfd) << endl;
	return true;
}

void printServerMessages() {
	while(AUTOLOAD) cout << readS(sockfd);
	while(!AUTOLOAD);
	printServerMessages();
}

void printFilteredServerMessages(regex filter) {
	smatch m;
	string answer;
	int c = 0;
	while(LOADFILTERED) {
		answer = readS(sockfd);
		if(regex_search(answer, m, filter)) {
			cout << c << ": ";
			cout << answer << endl;
			c++;
		}
	}
}

string getFilteredServerMessage(regex filter) {
	smatch m;
	string answer;
	int timeout = 5;
	int t = 0;
	while(t < timeout) {
		answer = readS(sockfd);
		if(regex_search(answer, m, filter)) {
			return answer.substr(answer.find("<12>"));
		}
		this_thread::sleep( posix_time::milliseconds(1000) );
		t++;
	}
	return "0";
}

string getServerAnswer() {
	return readS(sockfd);
}

bool saveGameToDB(string handle, int gameID) {
	cout << "Saving Game " << gameID << " from " << handle << " to Database..." << endl;
	string message; string answer;

	cout << "autoload off" << endl;
	AUTOLOAD = false;
	//usleep(1000000);

	/* Get Elo */
	message = "history " + handle + "\n";
	writeS(sockfd, message); // --> this one goes to printServerMessages() thread
	writeS(sockfd, message); // --> this one goes to answer
	answer = getServerAnswer();

	// Get Elo from history
	int whiteElo; int blackElo;
	size_t pos = answer.find(lexical_cast<string>(gameID) + ": ");
	if(gameID < 10) pos--; // Correct position for single digit numbers
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
		/*cout << "white Elo: " << whiteElo << endl;
		cout << "black Elo: " << blackElo << endl;*/
	}

	message = "examine " + handle + " " + lexical_cast<string>(gameID) + "\n";
	writeS(sockfd, message);

	regex style12(".*(<12> ).*$");
	vector<string> position(500);
	vector<string> fenstrings(8);
	Board board;
	smatch m;
	// Receives only the messages that match the filter style12
	//thread receiver (printFilteredServerMessages, style12);
	int i = 0;
	string white; string black;
	int GameID = 0; int PosID = 1;

	/* Skip initial position */
	writeS(sockfd, "forward\n");
	answer = getServerAnswer();

	// Scan positions to DataBase
	int new_p = 0; // new positions counter
	do {
		writeS(sockfd, "forward\n");
		answer = getServerAnswer(); //getFilteredServerMessage(style12);
		if(answer.find("<12>") == string::npos) break;
		position[i] = answer.substr(answer.find("<12>"));
		//cout << i << ": " << position[i] << endl;
		for (int j = 0; j < 8; j++) {
			fenstrings[j] = position[i].substr((5 + 9 * j), 8);
			//cout << "f"<< j << " " << fenstrings[j] << endl;
		}
		board = Board(Fen(fenstrings));
		//board.print();
		size_t offset = 0;
		for(int x = 0; x < 17; x++) {
			offset += position[i].substr(offset).find(' ');
			offset++;
			//cout << offset << endl;
		}
		white = position[i].substr(offset, position[i].substr(offset + 1).find(' ') + 1);
		offset += position[i].substr(offset).find(' ');
		black = position[i].substr(offset + 1, position[i].substr(offset + 1).find(' '));
		if(i == 0) {
			cout << endl;
			cout << "white: " <<  white << " (" << whiteElo << ")" << endl;
			cout << "black: " <<  black << " (" << blackElo << ")" << endl;
		}
		board.setPlayer(1, white, whiteElo);
		board.setPlayer(0, black, blackElo);

		if(i > 0) board.setGameID(GameID);

		board.setParent(PosID);

		// Write to DataBase
		if(board.writePositionToDB()) new_p++;
		//cout << board.getPositionIDFromDB() << endl;
		if(i == 0) {
			board.writePlayersToDB();
			board.writeGameToDB();
			GameID = board.getGameID();
		}
		PosID = board.getPositionIDFromDB();
		board.updateGame(PosID);
		i++;
	} while (answer.find("<12>") != string::npos); // answer.find("end of the game") == string::npos
	cout << new_p << " new Positions have been written to DB" << endl;

	//board[i] = Board(Fen(fenstrings));
	//LOADFILTERED = false;
	//cout << "autoload on" << endl;
	AUTOLOAD = true;
	writeS(sockfd, "unexamine\n");
	return true;
}

int main(int argc, char *argv[])
{
    int portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    /*if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }*/
    portno = 5000;
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

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    cout << "Connecting with " << host << "..." << endl;
    //fcntl(sockfd, F_SETFL, O_NONBLOCK);
    // Read welcome message from server
    readS(sockfd);

    // Entering Username
    cout << "Entering username " << endl;
	writeS(sockfd, "guest\n");

	// Read answer from Server
	readS(sockfd);

    // Entering Password
    cout << "Entering password " << endl;
	writeS(sockfd, "\n");

	// Read answer from Server
	readS(sockfd);
	cout << "Successfully connected with FICS-client skylex " << endl;

    // Set style-variable to 12
    cout << "setting style variable to 12 " << endl;
	writeS(sockfd, "style 12\n");

    cout << "setting style variable to 12 " << endl;
	writeS(sockfd, "set formula standard=1 & blitz=1\n");

	// Print continuously incoming messages from the server
	boost::thread output ( printServerMessages );

	// Wait for user input to send to the server
	while(true) {
		string input;
		//cin >> input;
		getline(cin, input);
		//cout << input + "\n";

		regex expr("^(savegame)[ ][A-z]+[ ][0-9]+");
		smatch m;
		if(regex_search(input, m, expr)) {
			size_t len; // length of handle
			len = input.substr(9).find(' ');
			string handle = input.substr(9, len);
			int gameID = boost::lexical_cast<int>(input.substr(10+len, input.length()));
			/*cout << "handle: " << handle << endl;
			cout << "gameID: " << gameID << endl;*/
			saveGameToDB(handle, gameID);
		} else writeS(sockfd, input + "\n");
	}
	exit(0);
	string answer;
	vector<string> fenstrings(8);
	vector<Board> board(100);
	int i = 0;
	// Scan positions
	do {
		writeS(sockfd, "forward\n");
		answer = readS(sockfd);
		cout << answer.substr(47, 72) << endl;
		//answer.substr(47, 72)
		for (int j = 0; j < 8; j++) {
			fenstrings[j] = answer.substr((47 + 9 * j), 8);
		}
		board[i] = Board(Fen(fenstrings));
		board[i].white = "skylex";
		board[i].black = "Kentinho";
		board[i].writePositionToDB();
		board[i].print();
		//fen[i].print();
		i++;
	} while (answer[0] == 'G');

	//cout << answer[1] << endl;
	/*while(true) {
		// Read answer from Server
		readS(sockfd);

		// Read Input and write it to socket
		string input;
		cin >> input;
		input.append("\n");
		writeS(sockfd, input);
	}*/
	close(sockfd);
    return 0;
}
