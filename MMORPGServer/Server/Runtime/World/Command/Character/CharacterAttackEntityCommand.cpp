#include "CharacterAttackEntityCommand.h"

namespace Server {

CharacterAttackEntityCommand::CharacterAttackEntityCommand( int idCharacter, int dx, int dy ) :
    WorldCommand( WorldCommandTypeEnum::CHARACTER_ATTACK_ENTITY ),
    _idCharacter( idCharacter ),
    _dx( dx ),
    _dy( dy ) {
}

int CharacterAttackEntityCommand::idCharacter() const {
    return _idCharacter;
}

int CharacterAttackEntityCommand::dx() const {
    return _dx;
}

int CharacterAttackEntityCommand::dy() const {
    return _dy;
}

} // namespace Server
