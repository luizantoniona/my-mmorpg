#ifndef GROUNDITEMEVENTLEAVEDTO_H
#define GROUNDITEMEVENTLEAVEDTO_H

#include <json/json.h>

namespace Engine {

class GroundItemEventLeaveDTO {
public:
    GroundItemEventLeaveDTO();
    ~GroundItemEventLeaveDTO();

    static GroundItemEventLeaveDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    int idGroundItem() const;
    void setIdGroundItem( int idGroundItem );

private:
    int _idGroundItem;
};

} // namespace Engine

#endif // GROUNDITEMEVENTLEAVEDTO_H
