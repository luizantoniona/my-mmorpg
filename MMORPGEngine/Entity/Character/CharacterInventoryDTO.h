#ifndef CHARACTERINVENTORYDTO_H
#define CHARACTERINVENTORYDTO_H

#include <cstdint>

#include <json/json.h>

#include <QString>

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

    QString idItem() const;
    void setIdItem( const QString& idItem );

    uint32_t amount() const;
    void setAmount( uint32_t amount );

private:
    QString _idItem;
    uint32_t _position;
    uint32_t _amount;
};

} // namespace Engine

#endif // CHARACTERINVENTORYDTO_H
