#include <string>
#include <mysql.h>
#ifndef DATABASE
#define DATABASE

using namespace std;

class Database {

protected:
     MYSQL mysql;
     string host;
     string user;
     string password;
     string database;
public:
    Database();
    Database(string host, string user, string password, string database);
};

#endif // DATABASE

