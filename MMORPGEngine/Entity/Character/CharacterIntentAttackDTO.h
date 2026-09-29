#ifndef CHARACTERINTENTATTACKDTO_H
#define CHARACTERINTENTATTACKDTO_H

#include <json/json.h>

namespace Engine {

class CharacterIntentAttackDTO {
public:
    CharacterIntentAttackDTO();
    ~CharacterIntentAttackDTO();

    static CharacterIntentAttackDTO fromJson( const Json::Value& json );
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

#endif // CHARACTERINTENTATTACKDTO_H
