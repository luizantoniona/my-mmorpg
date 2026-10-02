#include "OwnEquipmentDTO.h"

#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

OwnEquipmentDTO::OwnEquipmentDTO() :
    _equipment(),
    _idCharacter( 0 ) {
}

OwnEquipmentDTO OwnEquipmentDTO::fromModel( const CharacterModel* character ) {
    OwnEquipmentDTO dto;

    dto._idCharacter = character->idCharacter();

    dto._equipment.reserve( character->equipment().size() );

    for ( const CharacterEquipmentModel& equipment : character->equipment() ) {
        dto._equipment.push_back( CharacterEquipmentDTO::fromModel( equipment ) );
    }

    return dto;
}

OwnEquipmentDTO OwnEquipmentDTO::fromJson( const Json::Value& json ) {
    OwnEquipmentDTO dto;

    if ( json.isMember( "idCharacter" ) && json[ "idCharacter" ].isInt() ) {
        dto._idCharacter = json[ "idCharacter" ].asInt();
    }

    if ( json.isMember( "equipment" ) && json[ "equipment" ].isArray() ) {
        for ( const Json::Value& equipmentJson : json[ "equipment" ] ) {
            dto._equipment.push_back( CharacterEquipmentDTO::fromJson( equipmentJson ) );
        }
    }

    return dto;
}

Json::Value OwnEquipmentDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::OWN_EQUIPMENT );
    json[ "idCharacter" ] = _idCharacter;

    Json::Value equipmentJson( Json::arrayValue );

    for ( const CharacterEquipmentDTO& equipment : _equipment ) {
        equipmentJson.append( equipment.toJson() );
    }

    json[ "equipment" ] = equipmentJson;

    return json;
}

int OwnEquipmentDTO::idCharacter() const {
    return _idCharacter;
}

void OwnEquipmentDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

const std::vector<CharacterEquipmentDTO>& OwnEquipmentDTO::equipment() const {
    return _equipment;
}

void OwnEquipmentDTO::setEquipment( const std::vector<CharacterEquipmentDTO>& equipment ) {
    _equipment = equipment;
}

} // namespace Engine
