#include "MoveCharacterCommand.h"

#include <utility>

#include <MMORPGEngine/Commons/JsonHelper.h>
#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/Character/OwnCharacterDTO.h>
#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace Server {

MoveCharacterCommand::MoveCharacterCommand( int idCharacter, int dx, int dy, std::function<void( const std::string& )> respond ) :
    _respond( std::move( respond ) ),
    _idCharacter( idCharacter ),
    _dx( dx ),
    _dy( dy ) {
}

void MoveCharacterCommand::execute( WorldRuntime& runtime ) {
    Engine::CharacterModel* character = runtime.character( _idCharacter );
    if ( !character ) {
        return;
    }

    runtime.movementSystem().move( _idCharacter, _dx, _dy );

    _respond( Engine::JsonHelper::writeJsonString( Engine::OwnCharacterDTO::fromModel( character ).toJson() ) );
}

} // namespace Server
