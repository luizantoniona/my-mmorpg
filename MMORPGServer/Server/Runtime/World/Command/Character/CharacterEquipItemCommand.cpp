#include "CharacterEquipItemCommand.h"

namespace Server {

CharacterEquipItemCommand::CharacterEquipItemCommand( int idCharacter, uint32_t inventoryPosition ) :
    WorldCommand( WorldCommandTypeEnum::CHARACTER_EQUIP_ITEM ),
    _idCharacter( idCharacter ),
    _inventoryPosition( inventoryPosition ) {
}

int CharacterEquipItemCommand::idCharacter() const {
    return _idCharacter;
}

uint32_t CharacterEquipItemCommand::inventoryPosition() const {
    return _inventoryPosition;
}

} // namespace Server
