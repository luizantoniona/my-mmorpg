#include "AttackCharacterCommand.h"

#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace Server {

AttackCharacterCommand::AttackCharacterCommand( int idCharacter, int dx, int dy ) :
    _idCharacter( idCharacter ),
    _dx( dx ),
    _dy( dy ) {
}

void AttackCharacterCommand::execute( WorldRuntime& runtime ) {
    runtime.combatSystem().attack( _idCharacter, _dx, _dy );
}

} // namespace Server
