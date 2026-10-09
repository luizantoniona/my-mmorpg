#include "UnequipCharacterCommand.h"

#include <MMORPGServer/Server/Runtime/World/WorldRuntime.h>

namespace Server {

UnequipCharacterCommand::UnequipCharacterCommand( int idCharacter, Engine::EquipmentSlotEnum slot ) :
    _slot( slot ),
    _idCharacter( idCharacter ) {
}

void UnequipCharacterCommand::execute( WorldRuntime& runtime ) {
    runtime.unequipCharacter( _idCharacter, _slot );
}

} // namespace Server
