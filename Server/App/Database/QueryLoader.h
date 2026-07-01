#ifndef QUERYLOADER_H
#define QUERYLOADER_H

#include <string>

namespace Server {

class QueryLoader {
public:
    static std::string loadFromFile( const std::string& path );
    static std::string loadFromString( const std::string& sql );
};

} // namespace Server

#endif // QUERYLOADER_H
