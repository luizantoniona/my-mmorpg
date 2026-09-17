#ifndef WORLDBASICDTO_H
#define WORLDBASICDTO_H

#include <string>

#include <json/json.h>

namespace Engine {

class WorldBasicDTO {
public:
    WorldBasicDTO();
    ~WorldBasicDTO();

    static WorldBasicDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    std::string worldName() const;
    void setWorldName( const std::string& worldName );

private:
    std::string _worldName;
};

} // namespace Engine

#endif // WORLDBASICDTO_H
