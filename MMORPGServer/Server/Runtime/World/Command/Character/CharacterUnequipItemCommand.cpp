#include "CharacterUnequipItemCommand.h"

namespace Server {

CharacterUnequipItemCommand::CharacterUnequipItemCommand( int idCharacter, Engine::EquipmentSlotEnum slot ) :
    WorldCommand( WorldCommandTypeEnum::CHARACTER_UNEQUIP_ITEM ),
    _slot( slot ),
    _idCharacter( idCharacter ) {
}

int CharacterUnequipItemCommand::idCharacter() const {
    return _idCharacter;
}

Engine::EquipmentSlotEnum CharacterUnequipItemCommand::slot() const {
    return _slot;
}

} // namespace Server
