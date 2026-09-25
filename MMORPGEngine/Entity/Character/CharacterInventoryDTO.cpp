#include "CharacterInventoryDTO.h"

namespace Engine {

CharacterInventoryDTO::CharacterInventoryDTO() :
    _position( 0 ),
    _idItem( 0 ),
    _amount( 0 ) {
}

CharacterInventoryDTO CharacterInventoryDTO::fromModel( const CharacterInventoryModel& inventory ) {
    CharacterInventoryDTO dto;

    dto._position = inventory.position();
    dto._idItem = inventory.idItem();
    dto._amount = inventory.amount();

    return dto;
}

CharacterInventoryDTO CharacterInventoryDTO::fromJson( const Json::Value& json ) {
    CharacterInventoryDTO dto;

    if ( json.isMember( "position" ) && json[ "position" ].isUInt() ) {
        dto._position = json[ "position" ].asUInt();
    }

    if ( json.isMember( "idItem" ) && json[ "idItem" ].isUInt() ) {
        dto._idItem = json[ "idItem" ].asUInt();
    }

    if ( json.isMember( "amount" ) && json[ "amount" ].isUInt() ) {
        dto._amount = json[ "amount" ].asUInt();
    }

    return dto;
}

Json::Value CharacterInventoryDTO::toJson() const {
    Json::Value json;

    json[ "position" ] = _position;
    json[ "idItem" ] = _idItem;
    json[ "amount" ] = _amount;

    return json;
}

uint32_t CharacterInventoryDTO::position() const {
    return _position;
}

void CharacterInventoryDTO::setPosition( uint32_t position ) {
    _position = position;
}

uint32_t CharacterInventoryDTO::idItem() const {
    return _idItem;
}

void CharacterInventoryDTO::setIdItem( uint32_t idItem ) {
    _idItem = idItem;
}

uint32_t CharacterInventoryDTO::amount() const {
    return _amount;
}

void CharacterInventoryDTO::setAmount( uint32_t amount ) {
    _amount = amount;
}

} // namespace Engine
