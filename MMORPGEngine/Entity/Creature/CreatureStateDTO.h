#ifndef CREATURESTATEDTO_H
#define CREATURESTATEDTO_H

#include <json/json.h>

#include <MMORPGEngine/Entity/EntityOrientationEnum.h>

namespace Engine {

class CreatureStateDTO {
public:
    CreatureStateDTO();
    ~CreatureStateDTO();

    static CreatureStateDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCreature() const;
    void setIdCreature( int idCreature );

    int x() const;
    void setX( int x );

    int y() const;
    void setY( int y );

    int z() const;
    void setZ( int z );

    EntityOrientationEnum orientation() const;
    void setOrientation( EntityOrientationEnum orientation );

private:
    int _idCreature;
    int _x;
    int _y;
    int _z;
    EntityOrientationEnum _orientation;
};

} // namespace Engine

#endif // CREATURESTATEDTO_H
