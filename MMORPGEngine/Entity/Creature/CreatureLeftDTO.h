#ifndef CREATURELEFTDTO_H
#define CREATURELEFTDTO_H

#include <json/json.h>

namespace Engine {

class CreatureLeftDTO {
public:
    CreatureLeftDTO();
    ~CreatureLeftDTO();

    static CreatureLeftDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCreature() const;
    void setIdCreature( int idCreature );

private:
    int _idCreature;
};

} // namespace Engine

#endif // CREATURELEFTDTO_H
