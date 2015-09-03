/*
 * fen.h
 *
 *  Created on: Apr 6, 2015
 *      Author: alex
 */
#ifndef FEN_H_
#define FEN_H_
#include <vector>
#include <string>
//#include <boost/lexical_cast.hpp>
using namespace std;

class Fen {
	std::vector<string> fenstring;
	bool isNumber(char c);
	string makeFen(string row);
    vector<Fen*> children;
    Fen* parent = NULL;
    void setParent(Fen* parent);
public:
	Fen();
	Fen(std::vector<string> pos);
    void addChild(Fen child);
    Fen* getLastChild();
    Fen* getParent();
    bool hasChildren();
    vector<Fen*> getChildren();
    Fen* getChild(int i);
    Fen* getLastPosition();
	string getPos(int i);
	string getRow(int i);
    string getFen(int i);
    vector<string> getFen();
    string getFenString();
    string getMove();
    char getActiveColor();
    int getMoveNr();
	std::vector<int> searchChessman(char man);
	char getChessman(int x, int y); // die Schachfigur auf Feld x/y
	void setChessman(char man, int x, int y); // eine Schachfigur auf dem Feld x/y positionieren
	void changeFen(int i, string row);
	void print();
    void setActiveColor(char c);
};

#endif /* FEN_H_ */
