#ifndef CHARACTERINVENTORYDTO_H
#define CHARACTERINVENTORYDTO_H

#include <cstdint>

#include <json/json.h>

#include <MMORPGEngine/Entity/Character/CharacterInventoryModel.h>

namespace Engine {

class CharacterInventoryDTO {
public:
    CharacterInventoryDTO();

    static CharacterInventoryDTO fromModel( const CharacterInventoryModel& inventory );
    static CharacterInventoryDTO fromJson( const Json::Value& json );
    Json::Value toJson() const;

    uint32_t position() const;
    void setPosition( uint32_t position );

    uint32_t idItem() const;
    void setIdItem( uint32_t idItem );

    uint32_t amount() const;
    void setAmount( uint32_t amount );

private:
    uint32_t _position;
    uint32_t _idItem;
    uint32_t _amount;
};

} // namespace Engine

#endif // CHARACTERINVENTORYDTO_H
