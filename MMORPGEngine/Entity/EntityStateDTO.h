#ifndef ENTITYSTATEDTO_H
#define ENTITYSTATEDTO_H

#include <string>

#include <json/json.h>

namespace Engine {

class EntityStateDTO {
public:
    EntityStateDTO();
    ~EntityStateDTO();

    static EntityStateDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    int x() const;
    void setX( int x );

    int y() const;
    void setY( int y );

    int z() const;
    void setZ( int z );

    std::string worldName() const;
    void setWorldName( const std::string& worldName );

private:
    int _idCharacter;
    int _x;
    int _y;
    int _z;
    std::string _worldName;
};

} // namespace Engine

#endif // ENTITYSTATEDTO_H
