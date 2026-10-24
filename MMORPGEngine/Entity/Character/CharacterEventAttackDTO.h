#ifndef CHARACTEREVENTATTACKDTO_H
#define CHARACTEREVENTATTACKDTO_H

#include <utility>
#include <vector>

#include <json/json.h>

namespace Engine {

class CharacterEventAttackDTO {
public:
    CharacterEventAttackDTO();
    ~CharacterEventAttackDTO();

    static CharacterEventAttackDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    int x() const;
    void setX( int x );

    int y() const;
    void setY( int y );

    int z() const;
    void setZ( int z );

    const std::vector<std::pair<int, int>>& cells() const;
    void setCells( const std::vector<std::pair<int, int>>& cells );

private:
    std::vector<std::pair<int, int>> _cells;
    int _idCharacter;
    int _x;
    int _y;
    int _z;
};

} // namespace Engine

#endif // CHARACTEREVENTATTACKDTO_H
