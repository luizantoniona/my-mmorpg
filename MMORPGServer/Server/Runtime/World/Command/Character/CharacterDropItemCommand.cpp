#include "CharacterDropItemCommand.h"

namespace Server {

CharacterDropItemCommand::CharacterDropItemCommand( int idCharacter, uint32_t inventoryPosition ) :
    WorldCommand( WorldCommandTypeEnum::CHARACTER_DROP_ITEM ),
    _idCharacter( idCharacter ),
    _inventoryPosition( inventoryPosition ) {
}

int CharacterDropItemCommand::idCharacter() const {
    return _idCharacter;
}

uint32_t CharacterDropItemCommand::inventoryPosition() const {
    return _inventoryPosition;
}

} // namespace Server
