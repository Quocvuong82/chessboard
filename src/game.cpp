#include "game.h"
size_t Game::NrOfGames = 0;

Game::Game()
{
    board = new Board(Fen ());
    NrOfGames++;
}

size_t Game::getNrOfGames() {
    return NrOfGames;
}

int Game::getGameID() {
    return gameID;
}

void Game::setGameID(int id) {
    gameID = id;
}

char Game::getActiveColor() {
    if(board->getActiveColor() % 2 == 0) activeColor = 'w';
    else activeColor = 'b';
    return activeColor;
}

void Game::setActiveColor(char c) {
    activeColor = c;
    //board->setActiveColor(c);
}
