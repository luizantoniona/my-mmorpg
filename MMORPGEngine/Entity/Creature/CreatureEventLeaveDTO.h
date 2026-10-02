#ifndef CREATUREEVENTLEAVEDTO_H
#define CREATUREEVENTLEAVEDTO_H

#include <json/json.h>

namespace Engine {

class CreatureEventLeaveDTO {
public:
    CreatureEventLeaveDTO();
    ~CreatureEventLeaveDTO();

    static CreatureEventLeaveDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idCreature() const;
    void setIdCreature( int idCreature );

private:
    int _idCreature;
};

} // namespace Engine

#endif // CREATUREEVENTLEAVEDTO_H
