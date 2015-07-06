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
#include <boost/lexical_cast.hpp>
using namespace std;

class Fen {
	std::vector<string> fenstring;
	bool isNumber(char c);
	string makeFen(string row);
public:
	Fen();
	Fen(std::vector<string> pos);
	string getPos(int i);
	string getRow(int i);
	string getFen(int i);
    char getActiveColor();
    int getMoveNr();
	std::vector<int> searchChessman(char man);
	char getChessman(int x, int y); // die Schachfigur auf Feld x/y
	void setChessman(char man, int x, int y); // eine Schachfigur auf dem Feld x/y positionieren
	void changeFen(int i, string row);
	void print();
};

#endif /* FEN_H_ */
