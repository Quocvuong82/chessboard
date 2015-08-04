#ifndef PLAYER_H
#define PLAYER_H
#include <string>

using namespace std;

class Player
{
public:
    Player();
    string getName();
private:
    string name;
    string FICShandle;
    int elo;
    int id;
    int fideID;
};

#endif // PLAYER_H
