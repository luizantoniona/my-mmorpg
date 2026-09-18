#ifndef CREATUREDTO_H
#define CREATUREDTO_H

#include <json/json.h>

#include <MMORPGEngine/Entity/Creature/CreatureModel.h>

namespace Engine {

class CreatureDTO {
public:
    CreatureDTO();
    ~CreatureDTO();

    static CreatureDTO fromModel( const CreatureModel* creature );
    static CreatureDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCreature() const;
    void setIdCreature( int idCreature );

    int x() const;
    void setX( int x );

    int y() const;
    void setY( int y );

    int z() const;
    void setZ( int z );

private:
    int _idCreature;
    int _x;
    int _y;
    int _z;
};

} // namespace Engine

#endif // CREATUREDTO_H
