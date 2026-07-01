#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <sqlite3.h>

namespace Server {

class Repository {
public:
    explicit Repository();

protected:
    sqlite3* _db;
};

} // namespace Server

#endif // REPOSITORY_H
