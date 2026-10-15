#include "DropCharacterCommand.h"

#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace Server {

DropCharacterCommand::DropCharacterCommand( int idCharacter, uint32_t inventoryPosition ) :
    _idCharacter( idCharacter ),
    _inventoryPosition( inventoryPosition ) {
}

void DropCharacterCommand::execute( WorldRuntime& runtime ) {
    runtime.dropCharacterItem( _idCharacter, _inventoryPosition );
}

} // namespace Server
