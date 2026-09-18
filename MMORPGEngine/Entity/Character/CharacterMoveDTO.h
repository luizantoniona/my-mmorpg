#ifndef CHARACTERMOVEDTO_H
#define CHARACTERMOVEDTO_H

#include <json/json.h>

namespace Engine {

class CharacterMoveDTO {
public:
    CharacterMoveDTO();
    ~CharacterMoveDTO();

    static CharacterMoveDTO fromJson( const Json::Value& json );
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

#endif // CHARACTERMOVEDTO_H
