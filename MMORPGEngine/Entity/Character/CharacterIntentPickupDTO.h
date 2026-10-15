#ifndef CHARACTERINTENTPICKUPDTO_H
#define CHARACTERINTENTPICKUPDTO_H

#include <optional>

#include <json/json.h>

namespace Engine {

class CharacterIntentPickupDTO {
public:
    CharacterIntentPickupDTO();
    ~CharacterIntentPickupDTO();

    static CharacterIntentPickupDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    const std::optional<int>& idGroundItem() const;
    void setIdGroundItem( int idGroundItem );

private:
    std::optional<int> _idGroundItem;
};

} // namespace Engine

#endif // CHARACTERINTENTPICKUPDTO_H
