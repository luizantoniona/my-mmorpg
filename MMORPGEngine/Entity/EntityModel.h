#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <MMORPGEngine/Entity/EntityMovementModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/EntitySizeModel.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>

namespace Engine {

class EntityModel {
public:
    EntityModel();
    ~EntityModel();

    EntityPositionModel& position();
    const EntityPositionModel& position() const;

    EntitySizeModel& size();
    const EntitySizeModel& size() const;

    EntityVitalsModel& vitals();
    const EntityVitalsModel& vitals() const;

    EntityMovementModel& movement();
    const EntityMovementModel& movement() const;

private:
    EntityPositionModel _position;
    EntitySizeModel _size;
    EntityVitalsModel _vitals;
    EntityMovementModel _movement;
};

} // namespace Engine

#endif // ENTITYMODEL_H
