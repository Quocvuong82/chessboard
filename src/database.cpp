#include "database.h"

Database::Database (string host, string user, string password, string database){
    this->host = host;
    this->user = user;
    this->password = password;
    this->database = database;
};
