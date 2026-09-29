#ifndef ENTITYATTACKEDDTO_H
#define ENTITYATTACKEDDTO_H

#include <json/json.h>

namespace Engine {

class EntityAttackedDTO {
public:
    EntityAttackedDTO();
    ~EntityAttackedDTO();

    static EntityAttackedDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    int x() const;
    void setX( int x );

    int y() const;
    void setY( int y );

    int z() const;
    void setZ( int z );

private:
    int _idCharacter;
    int _x;
    int _y;
    int _z;
};

} // namespace Engine

#endif // ENTITYATTACKEDDTO_H
