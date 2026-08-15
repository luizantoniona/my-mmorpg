#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <MMORPGEngine/Entity/EntityPositionModel.h>

namespace Engine {

class EntityModel {
public:
    EntityModel();
    ~EntityModel();

    EntityPositionModel position() const;
    void setPosition( const EntityPositionModel& position );

private:
    EntityPositionModel _position;
};

} // namespace Engine

#endif // ENTITYMODEL_H
