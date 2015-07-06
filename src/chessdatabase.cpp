#include "chessdatabase.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

ChessDatabase::ChessDatabase (string host, string user, string password, string database): Database(host, user, password, database) {}

vector<int> ChessDatabase::getEventIDs() {
    string query = "SELECT event_id FROM event ORDER BY event_id desc";
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),database.c_str(),0,NULL,0))
    {
        cout << "Failed to connect to database: Error: " << mysql_error(&mysql);
    }
    MYSQL_RES *res;
    mysql_real_query(&mysql, query.c_str(), query.length());
    res = mysql_use_result(&mysql);
    MYSQL_ROW row;
    vector<int> eventIDs;
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
