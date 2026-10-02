#ifndef CREATUREEVENTATTACKDTO_H
#define CREATUREEVENTATTACKDTO_H

#include <json/json.h>

namespace Engine {

class CreatureEventAttackDTO {
public:
    CreatureEventAttackDTO();
    ~CreatureEventAttackDTO();

    static CreatureEventAttackDTO fromJson( const Json::Value& json );
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

#endif // CREATUREEVENTATTACKDTO_H
