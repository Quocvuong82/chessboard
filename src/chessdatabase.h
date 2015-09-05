#include "database.h"
#include <string>
#include <vector>
#include "fen.h"
#ifndef CHESSDATABASE
#define CHESSDATABASE

class ChessDatabase : public Database {
public:
    ChessDatabase();
    ChessDatabase(string host, string user, string password, string database);
    vector<int> getEventIDs();
    vector<vector<int>> getGameIDsByEventID(int eventID);
    vector<vector<int>> getGameIDsByString(string str, int eventID = 0);
    vector<int> getPosIDsByGameID(int gameID);
    string getEventByID(int id);
    string getPlayerByID(int playerID);
    vector<string> getPlayersByGameID(int gameID);
    vector<string> getPlayersToScan(); // get players to scan for new games on FICS
    vector<int> getPosIDsByParent(int parent);
    double getEvaluation(int posID);
    Fen getPositionFromDBByID(int posID);
    string getMoves(int GameID);
    int getPositionIDFromDB(Fen pos);
    //void cleanUpDatabase();
protected:
    bool createChessDatabase();
private:
    vector<int> getPlayersByString(string str);
};

#endif // CHESSDATABASE
