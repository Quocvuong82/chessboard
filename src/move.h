/*
 * move.h
 *
 *  Created on: Apr 6, 2015
 *      Author: alex
 */

#ifndef MOVE_H_
#define MOVE_H_
#include <string>
#include "fen.h"

class Move {
	std::string movecmd;
	Fen position;
	int activeColor;
	void parseMoveCmd();
	int checkChar(char character);
	Fen castling();
	Fen kingqueen();
	Fen knight();
	Fen bishop();
	Fen rook();
	Fen NBR();
	void pawn();
	void replacement();
public:
    Move();
    Move(Fen pos, std::string movecommand, int activeC);
    static std::string coord(int x, int y);
	void print();
	std::string getCmd();
	Fen getPosition();
};



#endif /* MOVE_H_ */
