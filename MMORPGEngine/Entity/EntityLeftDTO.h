#ifndef ENTITYLEFTDTO_H
#define ENTITYLEFTDTO_H

#include <json/json.h>

namespace Engine {

class EntityLeftDTO {
public:
    EntityLeftDTO();
    ~EntityLeftDTO();

    static EntityLeftDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

private:
    int _idCharacter;
};

} // namespace Engine

#endif // ENTITYLEFTDTO_H
