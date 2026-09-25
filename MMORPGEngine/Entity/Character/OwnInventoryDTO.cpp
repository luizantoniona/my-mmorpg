#include "OwnInventoryDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

OwnInventoryDTO::OwnInventoryDTO() :
    _items(),
    _idCharacter( 0 ) {
}

OwnInventoryDTO OwnInventoryDTO::fromModel( const CharacterModel* character ) {
    OwnInventoryDTO dto;

    dto._idCharacter = character->idCharacter();

    dto._items.reserve( character->inventory().size() );

    for ( const CharacterInventoryModel& inventory : character->inventory() ) {
        dto._items.push_back( CharacterInventoryDTO::fromModel( inventory ) );
    }

    return dto;
}

OwnInventoryDTO OwnInventoryDTO::fromJson( const Json::Value& json ) {
    OwnInventoryDTO dto;

    if ( json.isMember( "idCharacter" ) && json[ "idCharacter" ].isInt() ) {
        dto._idCharacter = json[ "idCharacter" ].asInt();
    }

    if ( json.isMember( "items" ) && json[ "items" ].isArray() ) {
        for ( const Json::Value& itemJson : json[ "items" ] ) {
            dto._items.push_back( CharacterInventoryDTO::fromJson( itemJson ) );
        }
    }

    return dto;
}

Json::Value OwnInventoryDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::OWN_INVENTORY );
    json[ "idCharacter" ] = _idCharacter;

    Json::Value itemsJson( Json::arrayValue );

    for ( const CharacterInventoryDTO& item : _items ) {
        itemsJson.append( item.toJson() );
    }

    json[ "items" ] = itemsJson;

    return json;
}

int OwnInventoryDTO::idCharacter() const {
    return _idCharacter;
}

void OwnInventoryDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

const std::vector<CharacterInventoryDTO>& OwnInventoryDTO::items() const {
    return _items;
}

void OwnInventoryDTO::setItems( const std::vector<CharacterInventoryDTO>& items ) {
    _items = items;
}

} // namespace Engine
