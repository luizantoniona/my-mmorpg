#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <sqlite3.h>

namespace Engine {

class Repository {
public:
    explicit Repository();

protected:
    sqlite3* _db;
};

} // namespace Engine

#endif // REPOSITORY_H
