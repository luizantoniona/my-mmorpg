#ifndef CHARACTERATTACKDTO_H
#define CHARACTERATTACKDTO_H

#include <json/json.h>

namespace Engine {

class CharacterAttackDTO {
public:
    CharacterAttackDTO();
    ~CharacterAttackDTO();

    static CharacterAttackDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int dx() const;
    void setDx( int dx );

    int dy() const;
    void setDy( int dy );

private:
    int _dx;
    int _dy;
};

} // namespace Engine

#endif // CHARACTERATTACKDTO_H
