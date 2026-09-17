#ifndef OWNCHARACTERSTATEDTO_H
#define OWNCHARACTERSTATEDTO_H

#include <json/json.h>

#include <MMORPGEngine/Entity/EntityOrientationEnum.h>

namespace Engine {

class OwnCharacterStateDTO {
public:
    OwnCharacterStateDTO();
    ~OwnCharacterStateDTO();

    static OwnCharacterStateDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    int x() const;
    void setX( int x );

    int y() const;
    void setY( int y );

    int z() const;
    void setZ( int z );

    EntityOrientationEnum orientation() const;
    void setOrientation( EntityOrientationEnum orientation );

private:
    int _idCharacter;
    int _x;
    int _y;
    int _z;
    EntityOrientationEnum _orientation;
};

} // namespace Engine

#endif // OWNCHARACTERSTATEDTO_H
