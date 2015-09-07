#include "chessdatabase.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <fstream>

ChessDatabase::ChessDatabase() : Database("localhost", "root", "", "myChessDB") {
}

ChessDatabase::ChessDatabase (string host, string user, string password, string database): Database(host, user, password, database) {}

bool ChessDatabase::createChessDatabase() {
    cout << "create new chess database" << endl;
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),NULL,0,NULL,CLIENT_MULTI_STATEMENTS))
    {
        cout << "Failed to connect to MySQL-Server: Error: " << mysql_error(&mysql);
        return false;
    }

    string sql;
    ifstream myfile ("/home/alex/build-chessboard-Desktop_Qt_5_4_2_GCC_64bit-Debug/createChessDB.sql");
    if (myfile.is_open()) {
        string line, query;
        while ( getline (myfile,line) )
        {
            if(line.find(';') != string::npos) {
                query.append(line);
                cout << query << " ";
                if(mysql_real_query(&mysql, query.c_str(), query.length())) cout << "true" << endl; else cout << "false" << endl;
                query.clear();
            } else {
                if(line[0] != '-' && line[1] != '-') query.append(line + "\n");
            }
        }
        myfile.close();
      }

      else cout << "Create Database Error: Unable to open SQL-file";


    mysql_close(&mysql);
    bool result = true;
    return result;
}

vector<int> ChessDatabase::getEventIDs() {
    vector<int> eventIDs;
    string query = "SELECT event_id FROM event ORDER BY event_id desc";
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
        return eventIDs;
    }
    MYSQL_RES *res;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_use_result(&mysql);
    MYSQL_ROW row;
    while (row = mysql_fetch_row(res)) {
        eventIDs.push_back(boost::lexical_cast<int>(row[0]));
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    return eventIDs;
}

vector<int> ChessDatabase::getPosIDsByGameID(int gameID) {
    string query = "SELECT positions FROM game WHERE game_id=" + boost::lexical_cast<string>(gameID);
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
    }
    cout << host << " " << user << " " << password << " " << database << endl;
    MYSQL_RES *res;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_use_result(&mysql);
    MYSQL_ROW row;
    vector<int> posIDs;
    row = mysql_fetch_row(res);
    string positions = row[0];
    size_t c = positions.find(',');
    posIDs.push_back(1);
    while(c != string::npos) {
        posIDs.push_back(boost::lexical_cast<int>(positions.substr(0,c)));
        //cout << positions.substr(0,c) << endl;
        positions = positions.substr(c + 1);
        c = positions.find(',');
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    return posIDs;
}

int ChessDatabase::getPositionIDFromDB(Fen pos) {
    int posID = 0;
    //cout << "getting pos ID from DB: ";

    MYSQL_RES *res;
    MYSQL_ROW row;
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,"localhost","root","floppy","schach",0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
        return false;
    }
    string query = "SELECT position_id FROM position WHERE fen1 = '" + pos.getFen(0) + "' AND fen2 = '" + pos.getFen(1) + "' AND fen3 = '" + pos.getFen(2) +
    "' AND fen4 = '" + pos.getFen(3) + "' AND fen5 = '" + pos.getFen(4) + "' AND fen6 = '" +
    pos.getFen(5) + "' AND fen7 = '" + pos.getFen(6) + "' AND fen8 = '" + pos.getFen(7) + "'";

    if(!mysql_real_query(&mysql, query.c_str(), query.length())) {

        res = mysql_use_result(&mysql);

        row = mysql_fetch_row(res);
        if(row) {
            posID = boost::lexical_cast<int>(row[0]);
            //posID = static_cast<int>(row[0]);
            //cout << "PosID: " << posID << endl;
        }
        mysql_free_result(res);

    }

    mysql_close(&mysql);
    //cout << endl;
    return posID;
}

vector<int> ChessDatabase::getPlayersByString(string str) {
    string query = "SELECT player_id FROM player WHERE name LIKE '%" + str + "%'";
    //cout << query << endl;
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
    }
    MYSQL_RES *res;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_use_result(&mysql);
    MYSQL_ROW row;

    vector<int> ids;
    while (row = mysql_fetch_row(res)) {
        ids.push_back(boost::lexical_cast<int>(row[0]));
    }
    return ids;
}

vector<vector<int>> ChessDatabase::getGameIDsByString(string str, int eventID) {
    cout << "get gameIDs by string" << endl;
    size_t sp = str.find(" ");
    vector<int> playerIDs = getPlayersByString(str.substr(0, sp));
    vector<int> playerIDs2;
    cout << str.size() << " " << sp << endl;
    if(sp != string::npos && str.size() - 1 > sp) {
        playerIDs2 = getPlayersByString(str.substr(sp + 1));
    }
    cout << playerIDs2.size() - 1 << endl;
    vector<int> ids = playerIDs;

    vector<vector<int>> gameIDs;
    MYSQL_ROW row;
    MYSQL_RES *res;

    for(int j = 0; j < playerIDs2.size() || ((sp == string::npos || (sp != string::npos && str.size() - 1 == sp)) && j==0); j++) {
        for(int i = 0; i < ids.size(); i++) {
            string query = "SELECT game_id, white_id, black_id FROM game WHERE ((white_id=" + boost::lexical_cast<string>(ids[i]);
            if(sp != string::npos && str.size() - 1 > sp) query += " AND black_id=" + boost::lexical_cast<string>(playerIDs2[j]);
            query += ") OR (";
            query += "black_id=" + boost::lexical_cast<string>(ids[i]);
            if(sp != string::npos && str.size() - 1 > sp) query += " AND white_id=" + boost::lexical_cast<string>(playerIDs2[j]);
            query += "))";
            if(eventID > 0) query += " AND event_id=" + boost::lexical_cast<string>(eventID);
            query += " ORDER BY game_id desc";
            //cout << query << endl;
            mysql_real_query(&mysql, query.c_str(), query.length());
            res = mysql_use_result(&mysql);
            while (row = mysql_fetch_row(res)) {
                vector<int> id(3);
                id[0] = boost::lexical_cast<int>(row[0]);
                id[1] = boost::lexical_cast<int>(row[1]);
                id[2] = boost::lexical_cast<int>(row[2]);
                gameIDs.push_back(id);
            }
        }
    }
    return gameIDs;
}

vector<vector<int>> ChessDatabase::getGameIDsByEventID(int eventID) {
    string query = "SELECT game_id, white_id, black_id FROM game WHERE event_id=" + boost::lexical_cast<string>(eventID) + " ORDER BY game_id desc";
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
    }
    MYSQL_RES *res;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_use_result(&mysql);
    MYSQL_ROW row;

    vector<vector<int>> ids;
    while (row = mysql_fetch_row(res)) {
        vector<int> id(3);
        id[0] = boost::lexical_cast<int>(row[0]);
        id[1] = boost::lexical_cast<int>(row[1]);
        id[2] = boost::lexical_cast<int>(row[2]);
        ids.push_back(id);
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    return ids;
}

string ChessDatabase::getEventByID(int id) {
    string query = "SELECT name FROM event WHERE event_id=" + boost::lexical_cast<string>(id);
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
        return "";
    }
    MYSQL_RES *res;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_use_result(&mysql);
    MYSQL_ROW row;
    string event;
    row = mysql_fetch_row(res);
    event = boost::lexical_cast<string>(row[0]);
    mysql_free_result(res);
    mysql_close(&mysql);
    return event;
}

string ChessDatabase::getPlayerByID(int playerID) {
    string query = "SELECT name FROM player WHERE player_id=" + boost::lexical_cast<string>(playerID);
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
    }
    MYSQL_RES *res;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_use_result(&mysql);
    MYSQL_ROW row;
    string player;
    row = mysql_fetch_row(res);
    player = boost::lexical_cast<string>(row[0]);
    mysql_free_result(res);
    mysql_close(&mysql);
    return player;
}

vector<string> ChessDatabase::getPlayersByGameID(int id) {
    string query = "SELECT white_id, black_id FROM game WHERE game_id=" + boost::lexical_cast<string>(id);
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
    }
    MYSQL_RES *res;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_use_result(&mysql);
    MYSQL_ROW row;
    string event;
    row = mysql_fetch_row(res);
    event = boost::lexical_cast<string>(row[0]);
    string white = getPlayerByID(boost::lexical_cast<int>(row[0]));
    string black = getPlayerByID(boost::lexical_cast<int>(row[1]));
    mysql_free_result(res);
    mysql_close(&mysql);
    vector<string> players;
    players.push_back(white);
    players.push_back(black);
    return players;
}

vector<string> ChessDatabase::getPlayersToScan() {
    string query = "SELECT fics_handle FROM `player` WHERE player_id > 202 and `last_fics_scan` < Now() - 1555200";
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
    }
    MYSQL_RES *res;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_use_result(&mysql);
    MYSQL_ROW row;
    vector<string> handle;
    while(row = mysql_fetch_row(res)) {
        handle.push_back(row[0]);
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    return handle;
}

/*void ChessDatabase::cleanUpDatabase() {
    string query = "SELECT game_id, date FROM game WHERE date REGEXP '^[ ]' and game_id >1413";
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
    }
    MYSQL_RES *res;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_use_result(&mysql);
    MYSQL_ROW row;
    vector<string> date;
    vector<string> gameID;
    while(row = mysql_fetch_row(res)) {
        gameID.push_back(row[0]);
        date.push_back(row[1]);
    }
    for(int i = 0; i < date.size(); i++) {
        date[i].erase(0, 1);
        date[i].append("5");
        query = "UPDATE `schach`.`game` SET `date` = '" + date[i] + "' WHERE `game`.`game_id`=" + gameID[i];
        mysql_real_query(&mysql, query.c_str(), query.length());
    }
    //query = "UPDATE `schach`.`game` SET `date` = '" + date[i] + "' WHERE `game`.`game_id`=" + gameID[i];
}*/

vector<int> ChessDatabase::getPosIDsByParent(int parent) {
    string query = "SELECT position_id FROM position WHERE parent=" + boost::lexical_cast<string>(parent);
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
    }
    MYSQL_RES *res;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_use_result(&mysql);
    MYSQL_ROW row;
    vector<int> posIDs;
    while(row = mysql_fetch_row(res)) {
        posIDs.push_back(boost::lexical_cast<int>(row[0]));
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    return posIDs;
}

double ChessDatabase::getEvaluation(int posID) {
    string query = "SELECT game_id FROM game WHERE positions LIKE '%," + boost::lexical_cast<string>(posID) + ",%' AND result = '0-1'";
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
    }
    MYSQL_RES *res;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_store_result(&mysql);
    int WhiteWon = static_cast<int>(mysql_num_rows(res));
    mysql_free_result(res);

    query = "SELECT game_id FROM game WHERE positions LIKE '%," + boost::lexical_cast<string>(posID) + ",%' AND result = '1-0'";
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_store_result(&mysql);
    int BlackWon = static_cast<int>(mysql_num_rows(res));
    mysql_free_result(res);
    mysql_close(&mysql);
    //cout << WhiteWon << " / " << BlackWon << endl;
    if(WhiteWon == BlackWon) return 0;
    if(WhiteWon > BlackWon) return static_cast<double>(BlackWon)/static_cast<double>(WhiteWon); else
    return -1 * (static_cast<double>(WhiteWon)/static_cast<double>(BlackWon));

}

Fen ChessDatabase::getPositionFromDBByID(int posID) {
    cout << "get Position " << posID << endl;
    MYSQL_RES *res;
    MYSQL_ROW row;
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
    }
    string query = "SELECT fen1, fen2, fen3, fen4, fen5, fen6, fen7, fen8, active_color, castle, en_passant, parent, game_id FROM position WHERE position_id=" + boost::lexical_cast<string>(posID) + ';';

    //cout << query << endl;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_use_result(&mysql);
    row = mysql_fetch_row(res);
    vector<string> fenstrings(13);

    for(int i = 0; i < 11; i++) {
        fenstrings[i] = row[i];
        cout << fenstrings[i] << endl;
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    fenstrings[11] = "0";
    fenstrings[12] = "1";

    Fen fen(fenstrings);
    return fen;
}

string ChessDatabase::getMoves(int GameID) {
    string query = "SELECT moves FROM game WHERE game_id=" + boost::lexical_cast<string>(GameID);
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_use_result(&mysql);
    row = mysql_fetch_row(res);
    string moves = row[0];
    mysql_free_result(res);
    mysql_close(&mysql);
    return moves;
}
