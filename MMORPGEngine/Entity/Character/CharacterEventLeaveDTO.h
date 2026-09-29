#ifndef CHARACTEREVENTLEAVEDTO_H
#define CHARACTEREVENTLEAVEDTO_H

#include <json/json.h>

namespace Engine {

class CharacterEventLeaveDTO {
public:
    CharacterEventLeaveDTO();
    ~CharacterEventLeaveDTO();

    static CharacterEventLeaveDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

private:
    int _idCharacter;
};

} // namespace Engine

#endif // CHARACTEREVENTLEAVEDTO_H
