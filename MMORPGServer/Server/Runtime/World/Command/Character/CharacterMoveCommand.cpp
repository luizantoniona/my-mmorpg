#include "CharacterMoveCommand.h"

namespace Server {

CharacterMoveCommand::CharacterMoveCommand( int idCharacter, int dx, int dy ) :
    WorldCommand( WorldCommandTypeEnum::CHARACTER_MOVE ),
    _idCharacter( idCharacter ),
    _dx( dx ),
    _dy( dy ) {
}

int CharacterMoveCommand::idCharacter() const {
    return _idCharacter;
}

int CharacterMoveCommand::dx() const {
    return _dx;
}

int CharacterMoveCommand::dy() const {
    return _dy;
}

} // namespace Server
