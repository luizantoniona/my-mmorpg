#include "OwnCombatDTO.h"

#include <MMORPGEngine/Entity/CombatActionHelper.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

OwnCombatDTO::OwnCombatDTO() :
    _actions(),
    _idCharacter( 0 ) {
}

OwnCombatDTO OwnCombatDTO::fromModel( const CharacterModel* character ) {
    OwnCombatDTO dto;

    dto._idCharacter = character->idCharacter();
    dto._actions = character->combat().availableActions();

    return dto;
}

OwnCombatDTO OwnCombatDTO::fromJson( const Json::Value& json ) {
    OwnCombatDTO dto;

    if ( json.isMember( "idCharacter" ) && json[ "idCharacter" ].isInt() ) {
        dto._idCharacter = json[ "idCharacter" ].asInt();
    }

    if ( json.isMember( "actions" ) && json[ "actions" ].isArray() ) {
        for ( const Json::Value& actionJson : json[ "actions" ] ) {
            const auto action = CombatActionHelper::fromString( actionJson.asString() );
            if ( action ) {
                dto._actions.insert( *action );
            }
        }
    }

    return dto;
}

Json::Value OwnCombatDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::OWN_COMBAT );
    json[ "idCharacter" ] = _idCharacter;

    Json::Value actionsJson( Json::arrayValue );

    for ( CombatActionEnum action : _actions ) {
        actionsJson.append( CombatActionHelper::toString( action ) );
    }

    json[ "actions" ] = actionsJson;

    return json;
}

int OwnCombatDTO::idCharacter() const {
    return _idCharacter;
}

void OwnCombatDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

const std::set<CombatActionEnum>& OwnCombatDTO::actions() const {
    return _actions;
}

void OwnCombatDTO::setActions( const std::set<CombatActionEnum>& actions ) {
    _actions = actions;
}

} // namespace Engine
