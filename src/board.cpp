/*
 * board.cpp
 *
 *  Created on: Apr 6, 2015
 *      Author: alex
 */

#include <iostream>
#include <fstream>
#include <mysql.h>
#include "board.h"
#include "move.h"
#include <ctime>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <QLabel>
#include <QString>
#include <QGridLayout>
#include "QPainter"

using namespace std;
using namespace boost;

QString Board::PATH = "/home/alex/build-chessboard-Desktop_Qt_5_4_2_GCC_64bit-Debug/";

Board::Board() {
    /* Create the 64 squares of the board */
    //Grid = new QGridLayout;

    /*int i;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            i = y * 8 + x;
            squares.append(new QLabel);
            squares[i]->setText(QString::number(i));
            //Grid->addWidget(squares[i], 8 - y, x);
        }
    }*/
    int i;
    for (int y = 0; y < 8; y++) {
       for (int x = 0; x < 8; x++) {
           i = y * 8 + x;
           squares.append(new QSquare);
           if((y + x) % 2 == 0) squares[i]->setPixmap(square_d); else
               squares[i]->setPixmap(square_l);
       }
    }
	connectwithDB(); 	// Verbinde mit Datenbank

	MYSQL_RES *res;
	MYSQL_ROW row;
	//mysql_init(&mysql);
	//mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"your_prog_name");
	/*if (!mysql_real_connect(&mysql,"localhost","root","floppy","schach",0,NULL,0))
	{
		cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
	}*/

	/* Hole Position aus der Datenbank */
	string query = "SELECT fen1, fen2, fen3, fen4, fen5, fen6, fen7, fen8, parent FROM position WHERE position_id=1";
	/*query = "SELECT fen1, fen2, fen3, fen4, fen5, fen6, fen7, fen8 FROM position WHERE position_id=";
	query += argv[1];
	query += ";";*/
	parent = 1;

	if(!mysql_real_query(&mysql, query.c_str(), query.length())) {

		res = mysql_use_result(&mysql);

		row = mysql_fetch_row(res);
		vector<string> fenstrings(8);

		if(row) {
			for(int i = 0; i < 8; i++) {
				fenstrings[i] = row[i];
			}
			mysql_free_result(res);
			Fen fen(fenstrings);
			position = fen;
            //positions.push_back(position);
		} else {
			Fen fen;
			position = fen;
            //positions.push_back(position);
			parent = 1;
			writePositionToDB();
		}
	} else {
		Fen fen;
		position = fen;
        //positions.push_back(position);
		parent = 1;
		writePositionToDB();
	}
    initialPosition = Fen ();
    currentPosition = &initialPosition;
}

Board::Board(Fen pos) {

    /* Create the 64 squares of the board */
    QPixmap square_d = QPixmap(PATH + "textures/wood_d.png");
    QPixmap square_l = QPixmap(PATH + "textures/wood_l.png");

    int i;
    initSquares();
    Grid = new QGridLayout;
    Grid->setHorizontalSpacing(0);
    Grid->setVerticalSpacing(0);

    /* Board Labeling */
    /* Letters */
    int minWidth = 0;
    for (int i = 1; i < 9; i++) {
        QLabel* l = new QLabel;
        string str = boost::lexical_cast<string>(i);
        str[0] = str[0] + 16;
        l->setText(QString::fromStdString(str));
        l->setAlignment(Qt::AlignCenter);
        //l->setMinimumHeight(minWidth);
        Grid->addWidget(l, 9, i);
    }
    for (int i = 1; i < 9; i++) {
        QLabel* l = new QLabel;
        string str = boost::lexical_cast<string>(i);
        str[0] = str[0] + 16;
        l->setText(QString::fromStdString(str));
        l->setAlignment(Qt::AlignCenter);
        //l->setMinimumHeight(minWidth);
        Grid->addWidget(l, 0, i);
    }
    /* Numbers */
    for (int i = 1; i < 9; i++) {
        QLabel* l = new QLabel;
        l->setText(QString::fromStdString(boost::lexical_cast<string>(9 - i)));
        l->setAlignment(Qt::AlignCenter);
        //l->setMinimumWidth(minWidth);
        Grid->addWidget(l, i, 0);
    }
    for (int i = 1; i < 9; i++) {
        QLabel* l = new QLabel;
        l->setText(QString::fromStdString(boost::lexical_cast<string>(9 - i)));
        l->setAlignment(Qt::AlignCenter);
        //l->setMinimumWidth(minWidth);
        Grid->addWidget(l, i, 9);
    }

    /* Board Squares */
    for (int y = 0; y < 8; y++) {
       for (int x = 0; x < 8; x++) {
           i = y * 8 + x;
           Grid->addWidget(squares[i], 8 - y, x + 1);
       }
    }

    Grid->setMargin(0);
    //Grid->SetMinimumSize;

	//connectwithDB();
    initialPosition = pos;
    currentPosition = &initialPosition;
    position = pos;
    //positions.push_back(position);
	parent = 1;
    castleWK = true;
    castleWQ = true;
    castleBK = true;
    castleBQ = true;
    //setPosition(pos);
}

void Board::newGame() {
    //white = White.getName();
    //black = Black.getName();
    /*cout << white << " " << black << endl;
    writePlayersToDB();
    writeGameToDB();*/
}

void Board::print() {
    currentPosition->print();
}

void Board::show() {
    writePositionTosquares();
}

vector<string> Board::getMoveHistory() {
    return movehistory;
}

bool Board::setPosition(Fen pos) {
        bool newPos = false;
        for(int i = 0; i < 8; i++) {
           if(currentPosition->getFen(i).compare(pos.getFen(i))) newPos = true;
        }

    if(newPos) {
        cout << "adding child" << endl;
        currentPosition->addChild( pos );
        cout << "getting last child" << endl;
        currentPosition = currentPosition->getLastChild();
        cout << "add position to positions" << endl;
        positions.push_back(currentPosition);
        cout << "add position to movehistory" << endl;
        movehistory.push_back(currentPosition->getMove());
    }
    /*initialPosition = pos;
    currentPosition = &initialPosition;*/
    /*position = pos;
    positions.push_back(position);*/

    setActiveColor(pos.getActiveColor());
    string values = pos.getFen(9);
    if(values[0] == 'K') castleWK = true; else castleWK = false;
    if(values[1] == 'Q') castleWQ = true; else castleWQ = false;
    if(values[2] == 'k') castleBK = true; else castleBK = false;
    if(values[3] == 'q') castleBQ = true; else castleBQ = false;
    moveNr = boost::lexical_cast<int>(pos.getFen(12));
}

string Board::getFenstring() {
    string fenstr;
    string f;
    for (int i = 0; i < 8; i++) {
        f.clear();
        f = currentPosition->getFen(i);
        fenstr.append(f);
        if(i < 7) fenstr.append("/");
    }
    fenstr += " " + boost::lexical_cast<string>(getActiveColor());
    fenstr += " ";
    fenstr += currentPosition->getFen(9);
    /*if(castleWK) fenstr += "K"; else fenstr += "-";
    if(castleWQ) fenstr += "Q"; else fenstr += "-";
    if(castleBK) fenstr += "k"; else fenstr += "-";
    if(castleBQ) fenstr += "q"; else fenstr += "-";*/
    //fenstr += " KQkq - 1 1";
    fenstr += " - 1 ";
    fenstr += boost::lexical_cast<string>(moveNr);
    return fenstr;
}

void Board::initSquares(){
    square_d = QPixmap(PATH + "textures/wood_d.png");
    square_l = QPixmap(PATH + "textures/wood_l.png");
    pawn_w_l = QPixmap(PATH + "textures/pawn_white_l.png");
    pawn_w_d = QPixmap(PATH + "textures/pawn_white_d.png");
    pawn_b_l = QPixmap(PATH + "textures/pawn_black_l.png");
    pawn_b_d = QPixmap(PATH + "textures/pawn_black_d.png");
    rook_w_l = QPixmap(PATH + "textures/rook_white_l.png");
    rook_w_d = QPixmap(PATH + "textures/rook_white_d.png");
    knight_w_l = QPixmap(PATH + "textures/knight_white_l.png");
    knight_w_d = QPixmap(PATH + "textures/knight_white_d.png");
    rook_b_l = QPixmap(PATH + "textures/rook_black_l.png");
    rook_b_d = QPixmap(PATH + "textures/rook_black_d.png");
    knight_b_l = QPixmap(PATH + "textures/knight_black_l.png");
    knight_b_d = QPixmap(PATH + "textures/knight_black_d.png");
    queen_w_l = QPixmap(PATH + "textures/queen_white_l.png");
    queen_w_d = QPixmap(PATH + "textures/queen_white_d.png");
    queen_b_l = QPixmap(PATH + "textures/queen_black_l.png");
    queen_b_d = QPixmap(PATH + "textures/queen_black_d.png");
    king_w_l = QPixmap(PATH + "textures/king_white_l.png");
    king_w_d = QPixmap(PATH + "textures/king_white_d.png");
    king_b_l = QPixmap(PATH + "textures/king_black_l.png");
    king_b_d = QPixmap(PATH + "textures/king_black_d.png");
    bishop_w_l = QPixmap(PATH + "textures/bishop_white_l.png");
    bishop_w_d = QPixmap(PATH + "textures/bishop_white_d.png");
    bishop_b_l = QPixmap(PATH + "textures/bishop_black_l.png");
    bishop_b_d = QPixmap(PATH + "textures/bishop_black_d.png");

    int scale = 64;

    for(int i = 0; i < 12; i++) {
        pieces[i] = QPixmap::fromImage(QImage(scale,scale, QImage::Format_ARGB32));
        pieces[i].fill(Qt::transparent);
        pieces[i].scaled(scale,scale);
    }

    pawn_w_l.fromImage(QImage(scale,scale, QImage::Format_ARGB32));
    pawn_w_d.fromImage(QImage(scale,scale, QImage::Format_ARGB32));
    pawn_w_l.fill(Qt::transparent);
    pawn_w_d.fill(Qt::transparent);
    pawn_w_l.scaled(scale,scale);
    pawn_w_d.scaled(scale,scale);

    pawn_b_l.fromImage(QImage(scale,scale, QImage::Format_ARGB32));
    pawn_b_d.fromImage(QImage(scale,scale, QImage::Format_ARGB32));
    pawn_b_l.fill(Qt::transparent);
    pawn_b_d.fill(Qt::transparent);
    pawn_b_l.scaled(scale,scale);
    pawn_b_d.scaled(scale,scale);

    vector<QPainter*> painter;
    pawn_w = new QSvgWidget;
    pawn_w->load(QString (PATH + "textures/WhitePawn.svg"));
    pawn_w->resize(scale,scale);

    for(int i = 0; i < 12; i++) {
        piecesSVG[i] = new QSvgWidget;
    }
    piecesSVG[0]->load(QString (PATH + "textures/WhitePawn.svg"));
    piecesSVG[1]->load(QString (PATH + "textures/WhiteRook.svg"));
    piecesSVG[2]->load(QString (PATH + "textures/WhiteKnight.svg"));
    piecesSVG[3]->load(QString (PATH + "textures/WhiteBishop.svg"));
    piecesSVG[4]->load(QString (PATH + "textures/WhiteQueen.svg"));
    piecesSVG[5]->load(QString (PATH + "textures/WhiteKing.svg"));
    piecesSVG[6]->load(QString (PATH + "textures/BlackPawn.svg"));
    piecesSVG[7]->load(QString (PATH + "textures/BlackRook.svg"));
    piecesSVG[8]->load(QString (PATH + "textures/BlackKnight.svg"));
    piecesSVG[9]->load(QString (PATH + "textures/BlackBishop.svg"));
    piecesSVG[10]->load(QString (PATH + "textures/BlackQueen.svg"));
    piecesSVG[11]->load(QString (PATH + "textures/BlackKing.svg"));

    for(int i = 0; i < 12; i++) {
        piecesSVG[i]->resize(scale,scale);
        painter.push_back(new QPainter(&pieces[i]));
        piecesSVG[i]->render(painter[i], QPoint(), QRegion(), QWidget::DrawChildren);
    }

    int i;
    for (int y = 0; y < 8; y++) {
       for (int x = 0; x < 8; x++) {
           i = y * 8 + x;
           squares.append(new QSquare);
           if((y + x) % 2 == 0) squares[i]->setPixmap(square_d); else
               squares[i]->setPixmap(square_l);
               squares[i]->setFixedSize(scale,scale);
       }
    }
}

void Board::writePositionTosquares() {
    QString fenstr;
    for (int i = 7; i >= 0; i--) {
        fenstr.append(QString::fromStdString(currentPosition->getRow(i)));
    }
    //cout << "created QString fenstr" << endl;
    //cout << fenstr.toStdString() << endl;
    //for (int i = 0; i < 64; i++) {
    //cout << fenstr.toStdString();
    int i;
     for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            i = y * 8 + x;
            switch(fenstr[i].unicode()) {
            case static_cast<int>('P'):
                squares[i]->setPixmap(pieces[0]);
                break;
            case static_cast<int>('R'):
                squares[i]->setPixmap(pieces[1]);
                break;
            case static_cast<int>('N'):
                squares[i]->setPixmap(pieces[2]);
                break;
            case static_cast<int>('B'):
                squares[i]->setPixmap(pieces[3]);
                break;
            case static_cast<int>('Q'):
                squares[i]->setPixmap(pieces[4]);
                break;
            case static_cast<int>('K'):
                squares[i]->setPixmap(pieces[5]);
                break;
            case static_cast<int>('p'):;
                squares[i]->setPixmap(pieces[6]);
                break;
            case static_cast<int>('r'):
                squares[i]->setPixmap(pieces[7]);
                break;
            case static_cast<int>('n'):;
                squares[i]->setPixmap(pieces[8]);
                break;
            case static_cast<int>('b'):
                squares[i]->setPixmap(pieces[9]);
                break;
            case static_cast<int>('q'):
                squares[i]->setPixmap(pieces[10]);
                break;
            case static_cast<int>('k'):
                squares[i]->setPixmap(pieces[11]);
                break;
            default:;
                if(fenstr[i] == '0' || fenstr[i] == '-') {
                    QPixmap empty = QPixmap::fromImage(QImage(64,64, QImage::Format_ARGB32));
                    empty.fill(Qt::transparent);
                    squares[i]->setPixmap(empty);
                }
            }
            //squares[i]->setText(QString (fenstr[i]));
        }
    }
     //cout << endl;
    //cout << "wrote Position to Squares" << endl;

}

char Board::getActiveColor() {
    return currentPosition->getActiveColor();
    /*if(activeColor % 2 == 0) return 'w';
    else return 'b';*/
}

bool Board::addMetadata(string metadata) {
	size_t begin; size_t end;
	size_t v_begin; size_t v_end;
	size_t a_begin; size_t a_end;
	string metatag;
	vector<string> value(20);
	vector<string> attribute(20);
	int i = 0;
	cout << "Parse Meta Tags" << endl;
	/* Parse Meta-Tags */
	while (begin = metadata.find('[') != string::npos) {
	cout << i << endl;
	end = metadata.find(']'); // End of Meta-Tag
	metatag = metadata.substr(begin, end); // Seperate metatag from meta-String

	/* Attribute */
	a_end = metatag.find(' ');
	attribute[i] = metatag.substr(0, a_end);
	cout << attribute[i] << ": ";

	/* Value */
	v_begin = metatag.find('"');
	v_end = metatag.substr(v_begin + 1).find('"') + 1;
	value[i] = metatag.substr(v_begin + 1, v_end - 1);
	cout << value[i] << endl;

	/* Handle different Line Endings (\r\n or \n)*/
	if(static_cast<int>(metadata.at(end + 2)) == 10) metadata = metadata.substr(end + 3);
	else metadata = metadata.substr(end + 2);

	if(attribute[i] == "Event") event = value[i];
	else if(attribute[i] == "Site") site = value[i];
	else if(attribute[i] == "Date") date = value[i];
	else if(attribute[i] == "Round") {
		try { round = boost::lexical_cast<int>(value[i]); }
		catch(boost::bad_lexical_cast& e) {
			round = 0;
		}

	}
	else if(attribute[i] == "White") white = value[i];
	else if(attribute[i] == "Black") black = value[i];
	else if(attribute[i] == "Result") result = value[i];
	else if(attribute[i] == "WhiteElo") {
		try { whiteElo = boost::lexical_cast<int>(value[i]); }
		catch(boost::bad_lexical_cast& e) {
			whiteElo = 0;
		}

	}
	else if(attribute[i] == "BlackElo") {
		try { blackElo = boost::lexical_cast<int>(value[i]); }
		catch(boost::bad_lexical_cast& e) {
			blackElo = 0;
		}

	}
	else if(attribute[i] == "EventDate") EventDate = value[i];
	else if(attribute[i] == "ECO") ECO = value[i];

	i++;
	}

	/* Workaround for Escaping String */
	size_t pos = event.find('\'');
	if (pos!=string::npos) event.erase(pos, 1);

	addMetadataToDB();
	return true;
}

void Board::printMetadata() {
	cout << "White: " << white << endl;
	cout << "Black: " << black << endl;
}

bool Board::addMetadataToDB() {
	cout << "adding meta data to DB" << endl;
	int gameID = 0;

	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql,"localhost","root","floppy","schach",0,NULL,0))
	{
		cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
		return false;
	}
	writePlayerToDB(white, whiteElo);
	writePlayerToDB(black, blackElo);
	writeEventToDB(event);
	cout << "wrote Player to DB" << endl;
	int event_id = getEventIDFromDB(event);

	int white_id = getPlayerIDFromDB(white); int black_id = getPlayerIDFromDB(black);
	gameID = getGameIDFromDB(moves);
	cout << "got event_id, white_id and black_id from DB" << endl;
	if(gameID == 0) {
		string query = "INSERT INTO game (game_id, moves, white_id, black_id, event_id, round, result, white_elo, black_elo, eco)";
		query += "VALUES (NULL, '" + moves + "', '" + boost::lexical_cast<string>(white_id) +"', '" + boost::lexical_cast<string>(black_id) + "', '" + boost::lexical_cast<string>(event_id) + "', '" + boost::lexical_cast<string>(round) + "', '" + result + "', '" + boost::lexical_cast<string>(whiteElo) + "', '" + boost::lexical_cast<string>(blackElo) + "', '" + ECO + "')";

		cout << query << endl;
		cout << "writing Game to DB " << endl;
		mysql_real_query(&mysql, query.c_str(), query.length());
		GameID = getGameIDFromDB(moves); // Set the boards game id
		mysql_close(&mysql);
	} else {
		GameID = gameID; // Set the boards game id
	}
	cout << gameID << " " << GameID << endl;
	return true;
}

int Board::getGameIDFromDB(string moves) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	int GameID = 0;
	string query = "SELECT game_id FROM game WHERE moves = '" + moves + "'";

	if(!mysql_real_query(&mysql, query.c_str(), query.length())) {

		res = mysql_use_result(&mysql);

		row = mysql_fetch_row(res);
		if(row) {
			GameID = boost::lexical_cast<int>(row[0]);
			//posID = static_cast<int>(row[0]);
			cout << "GameID: " << GameID << endl;
		}
	}
	mysql_free_result(res);

	return GameID;
}

bool Board::writeEventToDB(string event) {
	int EventID = 0;
	EventID = getEventIDFromDB(event);
	if(EventID == 0) {
		string query = "INSERT INTO event (event_id, name, site, date) VALUES (NULL, '" + event + "', '" + site + "', '" + date + "')";
		//cout << query << endl;
		cout << "writing Event to DB " << endl;
		mysql_real_query(&mysql, query.c_str(), query.length());
	}
	return true;
}

int Board::getEventIDFromDB(string event) {
	//cout << "getting Event ID" << endl;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int EventID = 0;

	string query = "SELECT event_id FROM event WHERE name = '" + event + "'";
	cout << query << endl;
	if(!mysql_real_query(&mysql, query.c_str(), query.length())) {

		res = mysql_use_result(&mysql);

		row = mysql_fetch_row(res);
		if(row) {
			EventID = boost::lexical_cast<int>(row[0]);
			//posID = static_cast<int>(row[0]);
			cout << "EventID: " << EventID << endl;
		}
	}
	mysql_free_result(res);
	cout << "EventID: " << EventID << endl;
	return EventID;
}

bool Board::writePlayerToDB(string name, int elo) {
    return writePlayerToDB(name, elo, "", false);
}

bool Board::writePlayerToDB(string name, int elo, string handle, bool ficsScan) {
	int playerID = 0;
    //cout << "write Player to DB " << endl;
	playerID = getPlayerIDFromDB(name);
    string query;
	if(playerID == 0) {
        query = "INSERT INTO player (player_id, name, fide_id, elo, fics_handle, last_fics_scan) VALUES (NULL, '" + name + "', '', '" + boost::lexical_cast<string>(elo) + "', '" + handle + "'";
        if(ficsScan) query += ", Now()"; else query += ", '0000-00-00 00:00:00'";
        query += ")";
        //cout << query << endl;
        cout << "writing Player " + name + " to DB " << endl;
		mysql_real_query(&mysql, query.c_str(), query.length());
    } else if (ficsScan) {
        query = "UPDATE player SET last_fics_scan = Now() WHERE player_id =" + boost::lexical_cast<string>(playerID) + ";";
        //cout << query << endl;
        cout << "updating Player " << playerID << endl;
        mysql_real_query(&mysql, query.c_str(), query.length());
    }
	return true;
}

int Board::getPlayerIDFromDB(string player) {
    //cout << "get PlayerID From DB " << endl;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int playerID = 0;
	string query = "SELECT player_id FROM player WHERE name = '" + player + "'";

	if(!mysql_real_query(&mysql, query.c_str(), query.length())) {

		res = mysql_use_result(&mysql);

		row = mysql_fetch_row(res);
		if(row) {
			playerID = boost::lexical_cast<int>(row[0]);
			//posID = static_cast<int>(row[0]);
			//cout << "PlayerID: " << playerID << endl;
		}
	}
	mysql_free_result(res);
    //cout << "player ID: " << playerID << endl;
	return playerID;
}

void Board::addMoves(string moves) {
	this->moves = moves;
}

void Board::printMoves() {
	cout << moves << endl;
}

void Board::nextPos() {
    if(currentPosition->hasChildren()) {
        currentPosition = currentPosition->getLastChild();
    }

    vector<Fen*> children = currentPosition->getChildren();
        cout << children.size() << " Children: ";
        for(int i = 0; i < children.size(); i++) {
            cout << DB.getPositionIDFromDB(*children[i]) << " ";
        }
        cout << endl;
    /*position = *currentPosition->getLastChild();
    activeColor = currentPosition->getActiveColor();*/

    /*vector<Fen> children = currentPosition->getChildren();
    cout << children.size() << " Children: ";
    for(int i = 0; i < children.size(); i++) {
        cout << DB.getPositionIDFromDB(children[i]) << " ";
    }*/
}

void Board::nextPos(int index) {
    vector<Fen*> children = currentPosition->getChildren();
    if(currentPosition->hasChildren()) {
        currentPosition = children[index];
        activeColor = currentPosition->getActiveColor();
    }
        cout << children.size() << " Children: ";
        for(int i = 0; i < children.size(); i++) {
            cout << DB.getPositionIDFromDB(*children[i]) << " ";
        }
        cout << endl;
}
void Board::prevPos() {
    cout << "Parent" << DB.getPositionIDFromDB(*currentPosition->getParent()) << endl;
    currentPosition = currentPosition->getParent();
    //position = *currentPosition->getParent()->getParent()->getParent();
    activeColor = currentPosition->getActiveColor();

    vector<Fen*> children = currentPosition->getChildren();
        cout << children.size() << " Children: ";
        for(int i = 0; i < children.size(); i++) {
            cout << DB.getPositionIDFromDB(*children[i]) << " ";
        }
    cout << endl;
    /*vector<Fen> children = currentPosition->getChildren();
    cout << children.size() << " Children: ";
    for(int i = 0; i < children.size(); i++) {
        cout << nextCombochildren[i]) << " ";
    }*/
}

void Board::setPosition(int index) {
    if(index < positions.size()) {
        currentPosition = positions[index];
        CurrentPosIndex = index;
    }
}

void Board::loadGame(int GameID) {
    ofstream chessfile;

    chessfile.open("chessfile.chx", ios_base::app);
    if(!chessfile.is_open())
        cerr << "Chess-File could not be created" << endl;

    cout << "load game " << GameID << endl;
    vector<int> posIDs = DB.getPosIDsByGameID(GameID);
    if(posIDs.size() < 1) cout << "no Position IDs" << endl;
    //cout << "posIDs set" << endl;
    positions.clear();
    CurrentPosIndex = 0;
    initialPosition = DB.getPositionFromDBByID(posIDs[0]);
    currentPosition = &initialPosition;
    for(int i = 1; i < posIDs.size(); i++) {
        chessfile << currentPosition->getFenString() << endl;
        currentPosition->addChild(DB.getPositionFromDBByID(posIDs[i]));
        positions.push_back(currentPosition);
        currentPosition = currentPosition->getLastChild();
        movehistory.push_back(currentPosition->getMove());
    }
    currentPosition = &initialPosition;

    /*Fen* child;
    child = currentPosition;
    for(int i = 0; i < positions.size(); i++) {
        child->addChild(positions[i]);
        child = child->getLastChild();
    }*/
    cout << GameID << endl;
    string movestr = DB.getMoves(GameID);
    cout << movestr << endl;
    if(movestr.size() > 1) {
        vector<string> moves = getSplittedPGN(movestr);
        vector<string> halfmoves;
        for(int i = 0; i < moves.size(); i++) {
            cout << moves[i] << endl;
            size_t sp = moves[i].find(" ");
            if(sp == string::npos) sp = moves[i].find("\n");
            if(sp == 0) sp = moves[i].find("\n", 1);
            halfmoves.push_back(moves[i].substr(0, sp));
            halfmoves.push_back(moves[i].substr(sp + 1));
        }
        halfmoves.erase(halfmoves.begin()); // Remove inital empty move
        movehistory = halfmoves;
        movehistory.erase(movehistory.begin());
    }
    show();
}

vector<string> Board::getSplittedPGN(string pgn_raw) {
    if(pgn_raw.size() == 0) {
        return vector<string>();
    }
    regex expr("[0-9]+[.]"); // z.B. "13. ", "14. ",...
    regex newline("[\r][\n]");
    smatch m; smatch n;

    vector<string> pgn(100);
    vector<string> pgn2(100);

    int i = 1; int length;
    regex_search(pgn_raw, m, expr);
    pgn[i] = pgn_raw.substr(m.position() + m.length());
    while(regex_search(pgn[i], m, expr)) {
        length = m.position();
        pgn2[i] = pgn[i].substr(0, length);
        if(pgn2[i][0] == ' ') pgn2[i].erase(0,1); // Remove space-character at the beginning

        /* Suche nach Whitespace-Characters */
        if(regex_search(pgn2[i], n, newline)) {
            pgn2[i].replace(n.position(), n.length(), " ");
        }
        i++;
        pgn[i] = pgn[i - 1].substr(m.position() + m.length());
        while(pgn[i][0] == ' ' || pgn[i][0] == '\n' || pgn[i][0] == '\r') pgn[i].erase(pgn[i].begin());
    }
    pgn2[i] = pgn[i];

    pgn2.resize(i + 1);

    return pgn2;
}

/* einen Zug machen */
/*void Board::move(string movecmd) {
    movehistory.push_back(movecmd);
	Move move(position, movecmd, activeColor); // einen neuen Zug machen mit Ausgansposition, Zugbefehl und aktivem Spieler
    positions[CurrentPosIndex].addChild(move.getPosition());
    position = move.getPosition(); // neue Position übernehmen
    CurrentPosIndex = positions.size(); // Update Position Index to current position
    positions.push_back(position); // add position to position list (move history)

    cout << "PosIDs";
    for(int i = 0; i < positions.size(); i++) {
        cout << nextCombopositions[i]) << " ";
    }
    cout << endl;
    if(DBwrite) writePositionToDB();
	parent = getPositionIDFromDB();

	if(activeColor == 1) activeColor = 0; else activeColor = 1; // aktiviere den anderen Spieler
    show();

}*/

/* einen Zug machen */
void Board::move(string movecmd) {
    Move move(*currentPosition, movecmd, activeColor); // einen neuen Zug machen mit Ausgansposition, Zugbefehl und aktivem Spieler
    /*while(currentPosition->hasChildren()) {
        position = *currentPosition->getLastChild();
    }*/
    currentPosition->addChild(move.getPosition());
    char activeColor = currentPosition->getActiveColor();
    currentPosition = currentPosition->getLastChild();
    positions.push_back(currentPosition);
    movehistory.push_back(currentPosition->getMove());

    if(DBwrite) writePositionToDB();
    parent = getPositionIDFromDB();

    if(activeColor == 'w') currentPosition->setActiveColor('b'); else
        currentPosition->setActiveColor('w');
    //if(activeColor == 1) activeColor = 0; else activeColor = 1; // depecated (active color is now handled by position)
    show();

}

bool Board::updateGame(int posID) {
    cout << "updating Game " << GameID << " with " << posID << endl;
	MYSQL_RES *res;
	MYSQL_ROW row;
	string query; string positions;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql,"localhost","root","floppy","schach",0,NULL,0))
	{
		cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
		return false;
	}

	query = "SELECT positions FROM game WHERE game_id =" + boost::lexical_cast<string>(GameID);
	if(!mysql_real_query(&mysql, query.c_str(), query.length())) {

			res = mysql_use_result(&mysql);
			row = mysql_fetch_row(res);
			if(row) {
				positions = row[0];
			}
			mysql_free_result(res);

	}

	query = "UPDATE game SET positions = '" + positions;
	if(positions.length() > 0) query += ",";
	query += boost::lexical_cast<string>(posID) + "' WHERE game_id=" + boost::lexical_cast<string>(GameID);
    //cout << "Updating Game" << endl;
	mysql_real_query(&mysql, query.c_str(), query.length());

	mysql_close(&mysql);
	return true;
}

bool Board::connectwithDB() {
	mysql_init(&mysql);
	//mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"your_prog_name");
	if (!mysql_real_connect(&mysql,"localhost","root","floppy","schach",0,NULL,0))
	{
		cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
		return false;
	}
	return true;
}

bool Board::getPositionFromDBByID(int id) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,"localhost","root","floppy","schach",0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
        return false;
    }
    string query = "SELECT fen1, fen2, fen3, fen4, fen5, fen6, fen7, fen8, active_color, castle, en_passant, parent, game_id FROM position WHERE position_id=" + boost::lexical_cast<string>(id) + ';';

    //cout << query << endl;
    mysql_real_query(&mysql, query.c_str(), query.length());
	res = mysql_use_result(&mysql);
	row = mysql_fetch_row(res);
    vector<string> fenstrings(13);

    for(int i = 0; i < 11; i++) {
		fenstrings[i] = row[i];
        cout << fenstrings[i] << endl;
	}
    fenstrings[11] = "0";
    fenstrings[12] = "1";

	Fen fen(fenstrings);
    position = fen;
    initialPosition = fen;
    currentPosition = &initialPosition;
    //positions.push_back(position);
    setActiveColor(currentPosition->getActiveColor());
    string values = currentPosition->getFen(9);
    if(values[0] == 'K') castleWK = true; else castleWK = false;
    if(values[1] == 'Q') castleWQ = true; else castleWQ = false;
    if(values[2] == 'k') castleBK = true; else castleBK = false;
    if(values[3] == 'q') castleBQ = true; else castleBQ = false;
    moveNr = 1;
    mysql_close(&mysql);
    cout << "Eval.: " << DB.getEvaluation(id) << endl;
	return true;
}

string Board::getPlayerW() {
    return white;
}

string Board::getPlayerB() {
    return black;
}

int Board::getPositionIDFromDB() {
	int posID = 0;
	//cout << "getting Position ID from DB: ";

	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql,"localhost","root","floppy","schach",0,NULL,0))
	{
		cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
		return false;
	}
    string query = "SELECT position_id FROM position WHERE fen1 = '" + currentPosition->getFen(0) + "' AND fen2 = '" + currentPosition->getFen(1) + "' AND fen3 = '" + currentPosition->getFen(2) +
    "' AND fen4 = '" + currentPosition->getFen(3) + "' AND fen5 = '" + currentPosition->getFen(4) + "' AND fen6 = '" +
    currentPosition->getFen(5) + "' AND fen7 = '" + currentPosition->getFen(6) + "' AND fen8 = '" + currentPosition->getFen(7) + "'";

	if(!mysql_real_query(&mysql, query.c_str(), query.length())) {

		res = mysql_use_result(&mysql);

		row = mysql_fetch_row(res);
		if(row) {
			posID = boost::lexical_cast<int>(row[0]);
			//posID = static_cast<int>(row[0]);
			//cout << "PosID: " << posID << endl;
		}
		mysql_free_result(res);

	}

	mysql_close(&mysql);
	//cout << endl;
	return posID;
}

/*int Board::getPositionIDFromDB(Fen pos) {
    int posID = 0;
    //cout << "getting pos ID from DB: ";

    MYSQL_RES *res;
    MYSQL_ROW row;
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,"localhost","root","floppy","schach",0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
        return false;
    }
    string query = "SELECT position_id FROM position WHERE fen1 = '" + pos.getFen(0) + "' AND fen2 = '" + pos.getFen(1) + "' AND fen3 = '" + pos.getFen(2) +
    "' AND fen4 = '" + pos.getFen(3) + "' AND fen5 = '" + pos.getFen(4) + "' AND fen6 = '" +
    pos.getFen(5) + "' AND fen7 = '" + pos.getFen(6) + "' AND fen8 = '" + pos.getFen(7) + "'";

    if(!mysql_real_query(&mysql, query.c_str(), query.length())) {

        res = mysql_use_result(&mysql);

        row = mysql_fetch_row(res);
        if(row) {
            posID = boost::lexical_cast<int>(row[0]);
            //posID = static_cast<int>(row[0]);
            //cout << "PosID: " << posID << endl;
        }
        mysql_free_result(res);

    }

    mysql_close(&mysql);
    //cout << endl;
    return posID;
}*/

bool Board::writePositionToDB() {
	//MYSQL mysql;
	int pos = getPositionIDFromDB();

	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql,"localhost","root","floppy","schach",0,NULL,0))
	{
		cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
		return false;
	}
    //cout << "Position ID: " << pos << " " << "Game ID: " << GameID << endl;
	if(pos == 0) {
        string query = "INSERT INTO position (position_id, fen1, fen2, fen3, fen4, fen5, fen6, fen7, fen8, active_color, castle, en_passant, parent, game_id) VALUES (NULL, ";
		for(int i = 0; i < 8; i++) {
            query += '\'' + currentPosition->getFen(i) + "', ";
		}
        query += "\'";
        query += getActiveColor();
        query += "\', ";
        query += "\'";
        if(castleWK) query += "K"; else query += "-";
        if(castleWQ) query += "Q"; else query += "-";
        if(castleBK) query += "k"; else query += "-";
        if(castleBQ) query += "q"; else query += "-";
        query += "\',";
        query += "\'" + enPassant + "\',";
        query += "\'";
		query += boost::lexical_cast<string>(parent);
		//query += "1";
		query += "', '" + boost::lexical_cast<string>(GameID);
		query += "')";
        //cout << query << endl;
        //cout << "writing Position to DB " << endl;
        mysql_real_query(&mysql, query.c_str(), query.length());

		mysql_close(&mysql);

		return true;
	} else { parent = pos; }

	mysql_close(&mysql);

	return false;
}

bool Board::setPlayer(int color, string name, int elo) {
    if(color % 2) {
        white = name; whiteElo = elo;
    } else {
        black = name; blackElo = elo;
    }
    return true;
}

bool Board::setPlayer(int color, string name, int elo, string handle, bool ficsscan) {
    if(color % 2) {
        white = name; whiteElo = elo; handleW = handle; ficsScanW = ficsscan;
    } else {
        black = name; blackElo = elo; handleB = handle; ficsScanB = ficsscan;
    }
    return true;
}

bool Board::setGameID(int gameID) {
    GameID = gameID;
    return true;
}
bool Board::setParent(int parent) {
    this->parent = parent;
    return true;
}

bool Board::writePlayersToDB() {
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,"localhost","root","floppy","schach",0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
        return false;
    }
    writePlayerToDB(white, whiteElo, handleW, ficsScanW);
    writePlayerToDB(black, blackElo, handleB, ficsScanB);
    mysql_close(&mysql);
    return true;
}

bool Board::writeGameToDB() {
    string query;
    MYSQL_RES *res;
    MYSQL_ROW row;
    int gameID = 0;
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,"localhost","root","floppy","schach",0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
        return false;
    }
    int white_id = getPlayerIDFromDB(white); int black_id = getPlayerIDFromDB(black);
    int event_id = 49; // ICS Rated Chess Match
    event_id = 282; // Chessboard Match

    query = "SELECT game_id FROM game WHERE white_id=" + boost::lexical_cast<string>(white_id) + " and black_id=" + boost::lexical_cast<string>(black_id) + " and date='" + date + "'";
    //cout << query << endl;
    if(!mysql_real_query(&mysql, query.c_str(), query.length())) {

        res = mysql_use_result(&mysql);

        row = mysql_fetch_row(res);
        if(row) {
            gameID = boost::lexical_cast<int>(row[0]);
            //posID = static_cast<int>(row[0]);
            //cout << "PosID: " << posID << endl;
        }
        mysql_free_result(res);

    }

    if(gameID == 0) {
        query = "INSERT INTO game (game_id, moves, white_id, black_id, event_id, round, result, white_elo, black_elo, eco, date)";
        query += "VALUES (NULL, '" + moves + "', '" + boost::lexical_cast<string>(white_id) +"', '" + boost::lexical_cast<string>(black_id) + "', '" + boost::lexical_cast<string>(event_id) + "', '" + boost::lexical_cast<string>(round) + "', '" + result + "', '" + boost::lexical_cast<string>(whiteElo) + "', '" + boost::lexical_cast<string>(blackElo) + "', '" + ECO + "', '" + date + "')";

        //cout << query << endl;
        //cout << "writing Game to DB " << endl;

        mysql_real_query(&mysql, query.c_str(), query.length());
        //GameID = getGameIDFromDB(moves); // Set the boards game id
        GameID = getRecentGameIDFromDB(); // Set the boards game id
        mysql_close(&mysql);
        return true;
    } else {
        cout << "Game already in DB (game_id: " << boost::lexical_cast<string>(gameID) << ")" << endl;
        mysql_close(&mysql);
    }
    return false;
}

int Board::getRecentGameIDFromDB() {
    MYSQL_RES *res;
    MYSQL_ROW row;
    int GameID = 0;
    string query = "SELECT game_id FROM game ORDER BY game_id DESC LIMIT 1";

    if(!mysql_real_query(&mysql, query.c_str(), query.length())) {

        res = mysql_use_result(&mysql);

        row = mysql_fetch_row(res);
        if(row) {
            GameID = boost::lexical_cast<int>(row[0]);
            //posID = static_cast<int>(row[0]);
            cout << "GameID: " << GameID << endl;
        }
    }
    mysql_free_result(res);

    return GameID;
}

int Board::getGameID() {
    return GameID;
}

/*char Board::getActiveColor() {
    if(activeColor % 2 == 1) return 'b';
    else return 'w';
}*/

void Board::setActiveColor(char c) {
    currentPosition->setActiveColor(c);
    activeColor = 0;
    if(c == 'b') activeColor = 1;
}

void Board::setGameDate(string date) {
    this->date = date;
}

void Board::writePositionsToDB() {
    for(int i = 0; i < positions.size(); i++) {
        //position = positions[i];
        writePositionToDB();
        updateGame(getPositionIDFromDB());
    }
}

void Board::saveGame() {
    cout << "Saving Game ";
    writePlayersToDB();
    time_t result = std::time(nullptr);
    date = asctime(localtime(&result));
    writeGameToDB();
    cout << GameID << endl;
    writePositionsToDB();
}
