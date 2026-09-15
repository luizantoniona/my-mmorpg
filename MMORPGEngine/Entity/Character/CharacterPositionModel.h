#ifndef CHARACTERPOSITIONMODEL_H
#define CHARACTERPOSITIONMODEL_H

#include <MMORPGEngine/Entity/EntityPositionModel.h>

namespace Engine {

class CharacterPositionModel {
public:
    CharacterPositionModel();

    int idCharacter() const;
    void setIdCharacter( int idCharacter );

    EntityPositionModel position() const;
    void setPosition( const EntityPositionModel& position );

private:
    int _idCharacter;
    EntityPositionModel _position;
};

} // namespace Engine

#endif // CHARACTERPOSITIONMODEL_H
