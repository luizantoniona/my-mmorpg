#include "CharacterPickupItemCommand.h"

namespace Server {

CharacterPickupItemCommand::CharacterPickupItemCommand( int idCharacter, int idGroundItem ) :
    WorldCommand( WorldCommandTypeEnum::CHARACTER_PICKUP_ITEM ),
    _idCharacter( idCharacter ),
    _idGroundItem( idGroundItem ) {
}

int CharacterPickupItemCommand::idCharacter() const {
    return _idCharacter;
}

int CharacterPickupItemCommand::idGroundItem() const {
    return _idGroundItem;
}

} // namespace Server
