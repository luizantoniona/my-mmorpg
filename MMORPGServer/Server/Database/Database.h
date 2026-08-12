#ifndef DATABASE_H
#define DATABASE_H

#include <string>

#include <sqlite3.h>

namespace Server {

class Database {
public:
    Database();
    ~Database();

    bool initialize( const std::string& databasePath );

    bool create();
    bool migrate();

    sqlite3* database();

private:
    sqlite3* _database;
    std::string _databasePath;

    bool open();
    void close();
};

} // namespace Server

#endif // DATABASE_H
