#ifndef GAME_H
#define GAME_H
#include "board.h"

class Game
{
public:
    Game();
    Board* board;
    static size_t getNrOfGames();
    int getGameID();
    void setGameID(int id);
    char getActiveColor();
    void setActiveColor(char c);
private:
    static size_t NrOfGames;
    int gameID;
    char activeColor;
};

#endif // GAME_H
