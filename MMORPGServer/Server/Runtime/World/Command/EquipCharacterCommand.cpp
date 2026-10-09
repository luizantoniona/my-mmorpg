#include "EquipCharacterCommand.h"

#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace Server {

EquipCharacterCommand::EquipCharacterCommand( int idCharacter, uint32_t inventoryPosition ) :
    _idCharacter( idCharacter ),
    _inventoryPosition( inventoryPosition ) {
}

void EquipCharacterCommand::execute( WorldRuntime& runtime ) {
    runtime.equipCharacter( _idCharacter, _inventoryPosition );
}

} // namespace Server
