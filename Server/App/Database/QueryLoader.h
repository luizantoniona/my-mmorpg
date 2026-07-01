#ifndef QUERYLOADER_H
#define QUERYLOADER_H

#include <string>

namespace Engine {

class QueryLoader {
public:
    static std::string loadFromFile( const std::string& path );
    static std::string loadFromString( const std::string& sql );
};

} // namespace Engine

#endif // QUERYLOADER_H
