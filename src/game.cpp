#include "game.h"
#include <boost/lexical_cast.hpp>
size_t Game::NrOfGames = 0;

Game::Game()
{
    board = new Board(Fen ());
    NrOfGames++;
    movehistory = new QTreeWidget();
    movehistory->setWindowTitle("move list");
    //movehistory->setFixedWidth(150);
    movehistory->setColumnCount(3);
    movehistory->headerItem()->setText(0, "#");
    movehistory->headerItem()->setText(1, "white");
    movehistory->headerItem()->setText(2, "black");
    QObject::connect(movehistory, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(HistoryItemClicked(QTreeWidgetItem*, int)));
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

void Game::showMoveHistory() {
    vector<string> mvhist = board->getMoveHistory();
    cout << currmoveNr << endl;
    for(int i = 0; i < mvhist.size(); i++) {
        if(i % 2 == 0) movehistory->insertTopLevelItem(i/2, new QTreeWidgetItem());
        movehistory->topLevelItem(i / 2)->setText(0, QString::fromStdString(boost::lexical_cast<string>(i/2)));
        movehistory->topLevelItem(i / 2)->setText(1 + i % 2, QString::fromStdString(mvhist[i]));
    }
    //currmoveNr = mvhist.size();
    for(int i = 0; i < movehistory->columnCount(); i++) {
        movehistory->resizeColumnToContents(i);
    }
    movehistory->show();
}

void Game::move(string movecmd) {
    board->move(movecmd);
    showMoveHistory();
}

void Game::nextPos() {
    board->nextPos();
    board->show();
    currmoveNr++;
    showMoveHistory();
}

void Game::prevPos() {
    board->prevPos();
    board->show();
    currmoveNr--;
    showMoveHistory();
}

void Game::HistoryItemClicked(QTreeWidgetItem* item, int c) {
    int index;
    index = item->text(0).toInt() * 2 + (c - 1);
    cout << index << " ";
    cout << item->text(c).toStdString() << endl;
    board->setPosition(index);
    board->show();
}
