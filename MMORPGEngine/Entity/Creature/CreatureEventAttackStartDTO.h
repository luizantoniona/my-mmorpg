#ifndef CREATUREEVENTATTACKSTARTDTO_H
#define CREATUREEVENTATTACKSTARTDTO_H

#include <json/json.h>

namespace Engine {

class CreatureEventAttackStartDTO {
public:
    CreatureEventAttackStartDTO();
    ~CreatureEventAttackStartDTO();

    static CreatureEventAttackStartDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCreature() const;
    void setIdCreature( int idCreature );

    int x() const;
    void setX( int x );

    int y() const;
    void setY( int y );

    int z() const;
    void setZ( int z );

    double castSeconds() const;
    void setCastSeconds( double castSeconds );

private:
    double _castSeconds;
    int _idCreature;
    int _x;
    int _y;
    int _z;
};

} // namespace Engine

#endif // CREATUREEVENTATTACKSTARTDTO_H
