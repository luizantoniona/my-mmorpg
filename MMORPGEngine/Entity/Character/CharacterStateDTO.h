#ifndef CHARACTERSTATEDTO_H
#define CHARACTERSTATEDTO_H

#include <json/json.h>

#include <MMORPGEngine/Entity/EntityOrientationEnum.h>

namespace Engine {

class CharacterStateDTO {
public:
    CharacterStateDTO();
    ~CharacterStateDTO();

    static CharacterStateDTO fromJson( const Json::Value& json );
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

#endif // CHARACTERSTATEDTO_H
