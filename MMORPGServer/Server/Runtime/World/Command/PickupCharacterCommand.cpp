#include "PickupCharacterCommand.h"

#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace Server {

PickupCharacterCommand::PickupCharacterCommand( int idCharacter, int idGroundItem ) :
    _idCharacter( idCharacter ),
    _idGroundItem( idGroundItem ) {
}

void PickupCharacterCommand::execute( WorldRuntime& runtime ) {
    runtime.pickupCharacterItem( _idCharacter, _idGroundItem );
}

} // namespace Server
