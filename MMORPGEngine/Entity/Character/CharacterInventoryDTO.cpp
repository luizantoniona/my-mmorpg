#include "CharacterInventoryDTO.h"

namespace Engine {

CharacterInventoryDTO::CharacterInventoryDTO() :
    _idItem( "" ),
    _position( 0 ),
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

    if ( json.isMember( "idItem" ) && json[ "idItem" ].isString() ) {
        dto._idItem = QString::fromStdString( json[ "idItem" ].asString() );
    }

    if ( json.isMember( "amount" ) && json[ "amount" ].isUInt() ) {
        dto._amount = json[ "amount" ].asUInt();
    }

    return dto;
}

Json::Value CharacterInventoryDTO::toJson() const {
    Json::Value json;

    json[ "position" ] = _position;
    json[ "idItem" ] = _idItem.toStdString();
    json[ "amount" ] = _amount;

    return json;
}

uint32_t CharacterInventoryDTO::position() const {
    return _position;
}

void CharacterInventoryDTO::setPosition( uint32_t position ) {
    _position = position;
}

QString CharacterInventoryDTO::idItem() const {
    return _idItem;
}

void CharacterInventoryDTO::setIdItem( const QString& idItem ) {
    _idItem = idItem;
}

uint32_t CharacterInventoryDTO::amount() const {
    return _amount;
}

void CharacterInventoryDTO::setAmount( uint32_t amount ) {
    _amount = amount;
}

} // namespace Engine
