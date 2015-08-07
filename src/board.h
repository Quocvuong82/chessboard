/*
 * board.h
 *
 *  Created on: Apr 6, 2015
 *      Author: alex
 */

#ifndef BOARD_H_
#define BOARD_H_
#include <string>
#include <mysql.h>
#include "fen.h"
#include "board.h"
#include <QLabel>
#include <QVector>
#include <QGridLayout>
#include <QSvgWidget>
#include "qsquare.h"
#include "chessdatabase.h"
#include "player.h"

class Board {
    Player White;
    Player Black;
	string moves;
	int activeColor = 0;
    bool castleWK;
    bool castleWQ;
    bool castleBK;
    bool castleBQ;
    string enPassant = "-";
    int moveNr = 1;
    //int GameID;
	string event;
	string site;
	string date;
	int round;
	string white = "Player White";
	string black = "Player Black";
    string handleW;
    string handleB;
    bool ficsScanW;
    bool ficsScanB;
	string result;
	int whiteElo;
	int blackElo;
	string EventDate;
	string ECO;

	MYSQL mysql;

	int parent = 1;

    /* Database */
    bool DBwrite = false;
	bool connectwithDB();
    ChessDatabase DB = ChessDatabase("localhost", "root", "floppy", "schach");

    /* Database Methods */
    bool writePlayerToDB(string name, int elo);
    bool writePlayerToDB(string name, int elo, string handle, bool ficsScan);
	bool writeEventToDB(string event);
    bool addMetadataToDB();

	int getEventIDFromDB(string event);
	int getGameIDFromDB(string moves);
	int getPlayerIDFromDB(string player);

    void writePositionTosquares();

    int getRecentGameIDFromDB();

    void initSquares();
    QPixmap pieces[12];
    QSvgWidget* piecesSVG[12];
    QSvgWidget* pawn_w;
    QSvgWidget* pawn_b;
    QPixmap square_d;
    QPixmap square_l;
    QPixmap pawn_w_l;
    QPixmap pawn_w_d;
    QPixmap pawn_b_l;
    QPixmap pawn_b_d;
    QPixmap rook_w_l;
    QPixmap rook_w_d;
    QPixmap knight_w_l;
    QPixmap knight_w_d;
    QPixmap rook_b_l;
    QPixmap rook_b_d;
    QPixmap knight_b_l;
    QPixmap knight_b_d;
    QPixmap queen_w_l;
    QPixmap queen_w_d;
    QPixmap queen_b_l;
    QPixmap queen_b_d;
    QPixmap king_w_l;
    QPixmap king_w_d;
    QPixmap king_b_l;
    QPixmap king_b_d;
    QPixmap bishop_w_l;
    QPixmap bishop_w_d;
    QPixmap bishop_b_l;
    QPixmap bishop_b_d;

    vector<Fen*> positions;
    Fen initialPosition;
    vector<string> movehistory;
    vector<string> getSplittedPGN(string pgn_raw);
    void writePositionsToDB();
public:
    Fen* currentPosition;
    int GameID = 0;
    QGridLayout *Grid;
    QVector<QSquare*> squares;
    //char getActiveColor();
    void setActiveColor(char c);
    /* temporary public */
    bool getPositionFromDBByID(int id);
    int getPositionIDFromDB();
    //int getPositionIDFromDB(Fen pos);
    void saveGame();
    Fen position;
    int CurrentPosIndex = 0;
    vector<string> getMoveHistory();
    /* ---------------- */
    void nextPos();
    void nextPos(int index);
    void prevPos();

    void setGameDate(string date);
    char getActiveColor();
    string getPlayerW();
    string getPlayerB();
	void print();
    void show();
    string getFenstring();
	void move(string movecmd);
	bool addMetadata(string metadata);
	void addMoves(string moves);
	void printMoves();
	void printMetadata();
	Board();
	Board(Fen pos);

    bool setPlayer(int color, string name, int elo);
    bool setPlayer(int color, string name, int elo, string handle, bool ficsscan = false);
    bool setGameID(int gameID);
    bool setParent(int parent);
    bool setPosition(Fen pos);
    void setPosition(int index);
    void loadGame(int GameID);
    //void setPosition(int posID);
    bool writePositionToDB();
    bool writePlayersToDB();
    bool writeGameToDB();
    int getGameID();
    bool updateGame(int posID);
    void newGame();
};



#endif /* BOARD_H_ */
