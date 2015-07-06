/*
 * move.cpp
 *
 *  Created on: Apr 6, 2015
 *      Author: alex
 */
#include "move.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "boost/regex.hpp"

using namespace std;

Move::Move() {
	movecmd = "";
	activeColor = 0;
}
Move::Move(Fen pos, std::string movecommand, int activeC) {
	position = pos;
	movecmd = movecommand;
	activeColor = activeC;
	parseMoveCmd();
}

void Move::parseMoveCmd() {
    boost::regex expr("^[a-z][0-9][a-z][0-9]");
    boost::smatch m;
    if(boost::regex_search(movecmd, m, expr)) {
        cout << "Long algebraic" << endl;
        cout << movecmd << endl;
        /* Zielfeld */
        int x = static_cast<int>(movecmd[2]) - 97;
        int y = 8 - (static_cast<int>(movecmd[3]) - 48);
        int x_origin = static_cast<int>(movecmd[0]) - 97;
        int y_origin = 8 - (static_cast<int>(movecmd[1]) - 48);
        //char man = movecmd[0];

        cout << "Ziel-Feld besetzen" << x_origin << "/" << y_origin << " " << x << "/" << y << endl;
        /* Ziel-Feld mit Figur besetzen */
        position.setChessman(position.getChessman(x_origin, y_origin), x, y);

        cout << "Feld frei machen" << endl;
        /* Feld frei machen */
        position.setChessman('0', x_origin, y_origin);

        cout << "Figur ";
        cout << " zieht von " << coord(x_origin, y_origin) << " (" << x_origin << "/" << y_origin << ") nach " << coord(x, y) << " (" << x << "/" << y << ")" << endl;
    } else {
        /* Algebraic Notation */
        if(movecmd[2] == '=') replacement();
        else if(movecmd[0] == 'O') castling();
        else {
            switch (checkChar(movecmd[0])) {
            case 2: // Großbuchstabe (2) --> FIGURENZUG
                if(activeColor == 1) {
                    // Wenn schwarz am Zug ist (activeColor == 1) dann Figurenzeichen in Kleinbuchstaben umwandeln
                    transform(movecmd.begin(), movecmd.end(), movecmd.begin(), ::tolower);
                }
                if (movecmd[0] == 'K' || movecmd[0] == 'Q' || movecmd[0] == 'k' || movecmd[0] == 'q') {
                    kingqueen(); // der König oder die Dame zieht
                } else {
                    NBR(); // einer der Springer, Läufer oder Turm zieht
                }
                break;
            case 3: // Kleinbuchstabe (3) --> BAUERNZUG: Kleinbuchstaben deuten auf einen Bauernzug hin
                pawn();
            }
        }
    }
}

void Move::replacement() {
	cout << "Replacement" << endl;
	/* Zielfeld */
	int x = static_cast<int>(movecmd[0]) - 97;
	int y = 8 - (static_cast<int>(movecmd[1]) - 48);
	int x_origin; int y_origin;
	char man = movecmd[3];

	if(activeColor == 0) {
		position.setChessman('0', x + 1, y); // Ursprungsfeld frei machen
	} else {
		position.setChessman('0', x - 1, y); // Ursprungsfeld frei machen
	}

	/* Zielfelder besetzen */
	position.setChessman(man, x, y);
}

int Move::checkChar(char character) {
	int char_ascii = static_cast<int>(character);
	if(char_ascii > 48) {
		if(char_ascii < 58) return 1; // ACII-Bereich Zahlen
		if(char_ascii > 64) {
			if(char_ascii < 91) return 2; // ACII-Bereich Großbuchstaben
			if(char_ascii > 96 && char_ascii < 122) return 3; // ACII-Bereich Kleinbuchstaben
		}
	}
	return 0;
}

string Move::coord(int x, int y) {
	string coord;
	char x_coord; char y_coord;
	x_coord = static_cast<char>(x + 97);
	y_coord = static_cast<char>((8 - y) + 48);
	coord += x_coord;
	coord += y_coord;
	return coord;
}
Fen Move::castling() {
	if(!activeColor) { /* Weiß am Zug */
		position.setChessman('0', 4, 7); // Ursprungsfeld frei machen König
		if(movecmd == "O-O-O") {
			cout << "Damenseitige Rochade" << endl;
			position.setChessman('0', 0, 7); // Ursprungsfeld frei machen Turm

			/* Zielfelder besetzen */
			position.setChessman('K', 2, 7); // König
			position.setChessman('R', 3, 7); // Turm
		} else {
			cout << "Königseitige Rochade" << endl;
			position.setChessman('0', 7, 7); // Ursprungsfeld frei machen Turm

			/* Zielfelder besetzen */
			position.setChessman('K', 6, 7); // König
			position.setChessman('R', 5, 7); // Turm
		}
	} else { /* Schwarz am Zug */
		position.setChessman('0', 4, 0); // Ursprungsfeld frei machen König
		if(movecmd == "O-O-O") {
			cout << "Damenseitige Rochade" << endl;
			position.setChessman('0', 0, 0); // Ursprungsfeld frei machen Turm

			/* Zielfelder besetzen */
			position.setChessman('k', 2, 0); // König
			position.setChessman('r', 3, 0); // Turm
		} else {
			cout << "Königseitige Rochade" << endl;
			position.setChessman('0', 7, 0); // Ursprungsfeld frei machen Turm

			/* Zielfelder besetzen */
			position.setChessman('k', 6, 0); // König
			position.setChessman('r', 5, 0); // Turm
		}

	}
	return position;
}
Fen Move::NBR() {
	char man = movecmd[0];
	switch(man) {
	case 'N':
	case 'n':
		knight();
		break;
	case 'B':
	case 'b':
		bishop();
		break;
	case 'R':
	case 'r':
		rook();
		break;
	}
	return position;
}
Fen Move::kingqueen() {
	/* Zielfeld */
	int x = static_cast<int>(movecmd[1]) - 97;
	int y = 8 - (static_cast<int>(movecmd[2]) - 48);
	int x_origin; int y_origin;
	char man = movecmd[0];

	/* derzeitige Position der Dame/des Königs ermitteln */
	vector<int> pos(2);
	pos = position.searchChessman(man);
	x_origin = pos[0]; y_origin = pos[1];

	/* Feld frei machen */
	position.setChessman('0', pos[0], pos[1]);

	/* Ziel-Feld mit Dame/König besetzen */
	position.setChessman(man, x, y);
	if(man == 'Q' || man == 'q') cout << "Dame"; else cout << "König";
	cout << " zieht von " << coord(x_origin, y_origin) << " (" << x_origin << "/" << y_origin << ") nach " << coord(x, y) << " (" << x << "/" << y << ")" << endl;
	return position;
}
void Move::pawn() {
	int x; int y;
	int x_origin; int y_origin;
	int offset;


	x = static_cast<int>(movecmd[0]) - 97;

	/* Prüfe ob es sich um einen Zug vom Typ 'fe6' oder 'f6' handelt */
	if(movecmd.size() > 2) {
		y = 8 - (static_cast<int>(movecmd[2]) - 48);
		offset = (static_cast<int>(movecmd[1]) - 97) - x;
	} else {
		y = 8 - (static_cast<int>(movecmd[1]) - 48);
		offset = 0;
	}
	//cout << x << " " << y << " " << offset << endl;

	/* Ursprungsfeld ermitteln */
	int j = y;
	string row = "xxxxxxxx";
	row = position.getPos(j); // FEN-String 4 (x-Richtung)
	//cout << row << endl;
	char figure = row[x]; // Figur auf Feld D (y-Richtung)
	char P;
	//cout << activeColor << endl;
	if(!activeColor) P = 'P'; else P = 'p'; // Unterscheide zwischen weißen und schwarzen Bauern

	/* Prüfe ob auf dem Feld ein Bauer steht */
	while(static_cast<int>(figure) != static_cast<int>(P)) {
		row = position.getPos(j);
		figure = row[x];
		if(static_cast<int>(figure) == static_cast<int>(P)) {
			row[x] = '0'; // Setze Ursprungsfeld auf 0 (Bauer zieht weg und hinterlässt leeres Feld)
			position.changeFen(j, row);
			x_origin = x; y_origin = j;
		}
		char feld = row[static_cast<int>(movecmd[0]) - 97];
		if(!activeColor) j++; else j--; // bei schwarzen Bauern umgekehrte Suchrichtung
		if(j < 0) break;
	}
	if(movecmd.size() > 2) x = static_cast<int>(movecmd[0]) - 97;

	/* Wandle Koordinaten in menschenlesbare Form um */
	cout << "Bauer zieht von " << coord(x_origin, y_origin) << " (" << x_origin << "/" << y_origin << ") nach " << coord(x + offset, y) << " (" << x + offset << "/" << y << ")" << endl;

	/* Bauer auf dem Zielfeld positionieren */
	position.setChessman(P, (x + offset), y);
	/*row = position.getPos(y);
	row[x + offset] = P;
	position.changeFen(y, row);*/
}
Fen Move::bishop() {
	int x = static_cast<int>(movecmd[1]) - 97;
	int y = 8 - (static_cast<int>(movecmd[2]) - 48);
	int x_origin; int y_origin;
	char man = movecmd[0];

	for(int i = 0; i < 8; i++) {
		if(x + i < 8 && y + i < 8) {
			if(position.getChessman(x + i, y + i) == man) {
				position.setChessman('0', x + i, y + i);
				x_origin = x + i; y_origin = y + i;
				break;
			}
		}
	}
	for(int i = 0; i < 8; i++) {
		if(x + i < 8 && y - i >= 0) {
			if(position.getChessman(x + i, y - i) == man) {
				position.setChessman('0', x + i, y - i);
				x_origin = x + i; y_origin = y - i;
				break;
			}
		}
	}
	for(int i = 0; i < 8; i++) {
		if(x - i >= 0 && y + i < 8) {
			if(position.getChessman(x - i, y + i) == man) {
				position.setChessman('0', x - i, y + i);
				x_origin = x - i; y_origin = y + i;
				break;
			}
		}
	}
	for(int i = 0; i < 8; i++) {
		if(x - i >= 0 && y - i >= 0) {
			if(position.getChessman(x - i, y - i) == man) {
				position.setChessman('0', x - i, y - i);
				x_origin = x - i; y_origin = y - i;
				break;
			}
		}
	}

	/* Figur auf dem Zielfeld positionieren */
	position.setChessman(man, x, y);

	cout << "Läufer zieht von " << coord(x_origin, y_origin) << " (" << x_origin << "/" << y_origin << ") nach " << coord(x, y) << " (" << x << "/" << y << ")" << endl;
	return position;
}
Fen Move::knight() {
	int mark = 100;
	if(movecmd.size() > 3) {
		mark = static_cast<int>(movecmd[1]) - 97;
		movecmd = movecmd.substr(0,1) + movecmd.substr(2);
	}

	int x = static_cast<int>(movecmd[1]) - 97;
	int y = 8 - (static_cast<int>(movecmd[2]) - 48);
	int x_origin; int y_origin;
	char man = movecmd[0];
	cout << mark << endl;

	if(mark < 100) {
		if(x + 2 < 8 && mark == (x + 2)) {
			if(y + 1 < 8) { if(position.getChessman((x + 2),(y + 1)) == man) { position.setChessman('0', (x + 2), (y + 1)); x_origin = x + 2; y_origin = y + 1; }}
			if(y - 1 >= 0) { if(position.getChessman((x + 2),(y - 1)) == man) { position.setChessman('0', (x + 2), (y - 1)); x_origin = x + 2; y_origin = y - 1; }}
		}
		if(x - 2 >= 0 && mark == (x - 2)) {
			if(y + 1 < 8) { if(position.getChessman((x - 2),(y + 1)) == man) { position.setChessman('0', (x - 2), (y + 1)); x_origin = x - 2; y_origin = y + 1; }}
			if(y - 1 >= 0) { if(position.getChessman((x - 2),(y - 1)) == man) { position.setChessman('0', (x - 2), (y - 1)); x_origin = x - 2; y_origin = y - 1; }}
		}
		if(x + 1 < 8 && mark == (x + 1)) {
			if(y + 2 < 8) { if(position.getChessman((x + 1),(y + 2)) == man) { position.setChessman('0', (x + 1), (y + 2)); x_origin = x + 1; y_origin = y + 2; }}
			if(y - 2 >= 0) { if(position.getChessman((x + 1),(y - 2)) == man) { position.setChessman('0', (x + 1), (y - 2)); x_origin = x + 1; y_origin = y - 2; }}
		}
		if(x - 1 >= 0 && mark == (x - 1)) {
			if(y + 2 < 8) { if(position.getChessman((x - 1),(y + 2)) == man) { position.setChessman('0', (x - 1), (y + 2)); x_origin = x - 1; y_origin = y + 2; }}
			if(y - 2 >= 0) { if(position.getChessman((x - 1),(y - 2)) == man) { position.setChessman('0', (x - 1), (y - 2)); x_origin = x - 1; y_origin = y - 2; }}
		}
	} else {
		if(x + 2 < 8) {
			if(y + 1 < 8) { if(position.getChessman((x + 2),(y + 1)) == man) { position.setChessman('0', (x + 2), (y + 1)); x_origin = x + 2; y_origin = y + 1; }}
			if(y - 1 >= 0) { if(position.getChessman((x + 2),(y - 1)) == man) { position.setChessman('0', (x + 2), (y - 1)); x_origin = x + 2; y_origin = y - 1; }}
		}
		if(x - 2 >= 0) {
			if(y + 1 < 8) { if(position.getChessman((x - 2),(y + 1)) == man) { position.setChessman('0', (x - 2), (y + 1)); x_origin = x - 2; y_origin = y + 1; }}
			if(y - 1 >= 0) { if(position.getChessman((x - 2),(y - 1)) == man) { position.setChessman('0', (x - 2), (y - 1)); x_origin = x - 2; y_origin = y - 1; }}
		}
		if(x + 1 < 8) {
			if(y + 2 < 8) { if(position.getChessman((x + 1),(y + 2)) == man) { position.setChessman('0', (x + 1), (y + 2)); x_origin = x + 1; y_origin = y + 2; }}
			if(y - 2 >= 0) { if(position.getChessman((x + 1),(y - 2)) == man) { position.setChessman('0', (x + 1), (y - 2)); x_origin = x + 1; y_origin = y - 2; }}
		}
		if(x - 1 >= 0) {
			if(y + 2 < 8) { if(position.getChessman((x - 1),(y + 2)) == man) { position.setChessman('0', (x - 1), (y + 2)); x_origin = x - 1; y_origin = y + 2; }}
			if(y - 2 >= 0) { if(position.getChessman((x - 1),(y - 2)) == man) { position.setChessman('0', (x - 1), (y - 2)); x_origin = x - 1; y_origin = y - 2; }}
		}
	}
	/* Figur auf dem Zielfeld positionieren */
	position.setChessman(man, x, y);

	/* Wandle Koordinaten in menschenlesbare Form um */
	cout << "Springer zieht von " << coord(x_origin, y_origin) << " (" << x_origin << "/" << y_origin << ") nach " << coord(x, y) << " (" << x << "/" << y << ")" << endl;
	return position;
}
Fen Move::rook() {
	int x; int y; int mark;
	int x_origin; int y_origin;
	bool marker = false;
	bool Xgiven = false; // true --> x, false --> y
	if(movecmd.size() > 3) {
		marker = true;
		x = static_cast<int>(movecmd[2]) - 97;
		y = 8 - (static_cast<int>(movecmd[3]) - 48);

		/*if(static_cast<int>(movecmd[1]) > 57) mark = static_cast<int>(movecmd[1]) - 97; // x
		else mark = static_cast<int>(movecmd[1]) - 48; // y*/
		if(static_cast<int>(movecmd[1]) > 57) Xgiven = true;

		if(Xgiven) x_origin = static_cast<int>(movecmd[1]) - 97; // x z.B. Rhd8
		else y_origin = 8 - (static_cast<int>(movecmd[1]) - 48); // y z.B. R7c6
	} else {
		x = static_cast<int>(movecmd[1]) - 97;
		y = 8 - (static_cast<int>(movecmd[2]) - 48);
		mark = 0;
	}

	char man = movecmd[0];
	size_t pos;

	if(marker) {
		if(Xgiven) { // x-coordinate is Xgiven
			for(int i = 0; i < 8; i++) {
				if(position.getChessman(x_origin, i) == man) { // Prüfen ob Figur in Reihe i steht
					//cout << x_origin << "/" << i << endl;
					//cout << position.getChessman(x_origin, i) << endl;
					position.setChessman('0', x_origin, i); // Ursprungsfeld frei machen
					y_origin = i;
				}
			}
		} else {
			x_origin = x;
			position.setChessman('0', x_origin, y_origin); // Ursprungsfeld frei machen
		}
	} else {

		/* Prüfen ob in der aktuellen Reihe ($row) die Figur steht, die bewegt werden soll */
		size_t pos;
		string row = position.getRow(y);

		pos = row.find(man);
			if(pos!=string::npos) {
				cout << "set chessman " << pos << "/" << y << endl;
				position.setChessman('0', pos, y); // Ursprungsfeld frei machen (da Figur ja wegzieht und somit ein Feld frei wird)
				x_origin = pos; y_origin = y;
			} else {
				/* In Spalten suchen */
				for(int i = 0; i < 8; i++) {
					if(position.getChessman(x, i) == man) { // Prüfen ob Figur in aktueller Reihe steht
						position.setChessman('0', x, i); // Ursprungsfeld frei machen
						x_origin = x; y_origin = i;
					}
				}
			}

		pos = row.substr(pos + 1).find(man) + pos + 1; // Check 2nd Rook (if there is any)

		if(pos!=string::npos) {
				position.setChessman('0', pos, y); // Ursprungsfeld frei machen (da Figur ja wegzieht und somit ein Feld frei wird)
				x_origin = pos; y_origin = y;
		} else {
			/* In Spalten suchen */
			for(int i = 0; i < 8; i++) {
				if(position.getChessman(x, i) == man) { // Prüfen ob Figur in aktueller Reihe steht
					position.setChessman('0', x, i); // Ursprungsfeld frei machen
					x_origin = x; y_origin = i;
				}
			}
		}
	}

	/* Figur auf dem Zielfeld positionieren */
	position.setChessman(man, x, y);

	/* Wandle Koordinaten in menschenlesbare Form um */
	cout << "Turm zieht von " << coord(x_origin, y_origin) << " (" << x_origin << "/" << y_origin << ") nach " << coord(x, y) << " (" << x << "/" << y << ")" << endl;
	return position;
}

void Move::print() {
	cout << movecmd << endl;
}
std::string Move::getCmd() {
	return movecmd;
}

Fen Move::getPosition() {
	return position;
}
