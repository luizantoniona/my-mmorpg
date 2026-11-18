#include "CharacterAppearanceDTO.h"

#include <MMORPGEngine/Entity/Character/CharacterAppearanceFeatureHelper.h>
#include <MMORPGEngine/Network/WebSocket/ServerMessageTypeHelper.h>

namespace Engine {

CharacterAppearanceDTO::CharacterAppearanceDTO() :
    _appearance(),
    _idCharacter( 0 ) {
}

CharacterAppearanceDTO::~CharacterAppearanceDTO() = default;

CharacterAppearanceDTO CharacterAppearanceDTO::fromModel( const CharacterModel* character ) {
    CharacterAppearanceDTO dto;

    dto._idCharacter = character->idCharacter();
    dto._appearance = character->appearance();

    return dto;
}

CharacterAppearanceDTO CharacterAppearanceDTO::fromJson( const Json::Value& json ) {
    CharacterAppearanceDTO dto;

    if ( json.isMember( "idCharacter" ) && json[ "idCharacter" ].isInt() ) {
        dto._idCharacter = json[ "idCharacter" ].asInt();
    }

    if ( json.isMember( "appearance" ) ) {
        selectionFromJson( json[ "appearance" ], dto._appearance );
    }

    return dto;
}

Json::Value CharacterAppearanceDTO::toJson() const {
    Json::Value json;

    json[ "type" ] = ServerMessageTypeHelper::toString( ServerMessageType::CHARACTER_APPEARANCE );
    json[ "idCharacter" ] = _idCharacter;
    json[ "appearance" ] = selectionToJson( _appearance );

    return json;
}

Json::Value CharacterAppearanceDTO::selectionToJson( const CharacterAppearanceModel& appearance ) {
    Json::Value json( Json::objectValue );

    for ( const auto& selection : appearance.selections() ) {
        json[ CharacterAppearanceFeatureHelper::toString( selection.first ) ] = selection.second.toStdString();
    }

    return json;
}

bool CharacterAppearanceDTO::selectionFromJson( const Json::Value& json, CharacterAppearanceModel& appearance ) {
    if ( !json.isObject() ) {
        return false;
    }

    CharacterAppearanceModel parsed;

    for ( const std::string& featureName : json.getMemberNames() ) {
        const CharacterAppearanceFeatureEnum feature = CharacterAppearanceFeatureHelper::fromString( featureName );
        const Json::Value& idJson = json[ featureName ];

        if ( feature == CharacterAppearanceFeatureEnum::UNKNOWN || !idJson.isString() || idJson.asString().empty() ) {
            return false;
        }

        parsed.setId( feature, QString::fromStdString( idJson.asString() ) );
    }

    appearance = parsed;

    return true;
}

int CharacterAppearanceDTO::idCharacter() const {
    return _idCharacter;
}

void CharacterAppearanceDTO::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

const CharacterAppearanceModel& CharacterAppearanceDTO::appearance() const {
    return _appearance;
}

void CharacterAppearanceDTO::setAppearance( const CharacterAppearanceModel& appearance ) {
    _appearance = appearance;
}

} // namespace Engine
