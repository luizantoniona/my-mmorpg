#ifndef CHARACTERINTENTMOVEDTO_H
#define CHARACTERINTENTMOVEDTO_H

#include <json/json.h>

namespace Engine {

class CharacterIntentMoveDTO {
public:
    CharacterIntentMoveDTO();
    ~CharacterIntentMoveDTO();

    static CharacterIntentMoveDTO fromJson( const Json::Value& json );
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

#endif // CHARACTERINTENTMOVEDTO_H
