#include "CharacterPositionModel.h"

namespace Engine {

CharacterPositionModel::CharacterPositionModel() :
    _idCharacter( 0 ),
    _position() {
}

int CharacterPositionModel::idCharacter() const {
    return _idCharacter;
}

void CharacterPositionModel::setIdCharacter( int idCharacter ) {
    _idCharacter = idCharacter;
}

EntityPositionModel CharacterPositionModel::position() const {
    return _position;
}

void CharacterPositionModel::setPosition( const EntityPositionModel& position ) {
    _position = position;
}

} // namespace Engine
