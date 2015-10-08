#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#include "engine.h"

class ChessEngine : public Engine
{
Q_OBJECT
public:
    ChessEngine(QString pathToEngine);
    void setPosition(string fen);
    vector<vector<string>> getOtherMoves();
private:
    string fen;
signals:
public slots:
    void go();
    void stop();
    void getValues(QString);
};

#endif // CHESSENGINE_H
