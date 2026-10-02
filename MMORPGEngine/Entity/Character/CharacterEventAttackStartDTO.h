#ifndef CHARACTEREVENTATTACKSTARTDTO_H
#define CHARACTEREVENTATTACKSTARTDTO_H

#include <json/json.h>

namespace Engine {

class CharacterEventAttackStartDTO {
public:
    CharacterEventAttackStartDTO();
    ~CharacterEventAttackStartDTO();

    static CharacterEventAttackStartDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

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
    int _idCharacter;
    int _x;
    int _y;
    int _z;
};

} // namespace Engine

#endif // CHARACTEREVENTATTACKSTARTDTO_H
