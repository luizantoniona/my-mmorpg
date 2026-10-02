#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <MMORPGEngine/Entity/EntityCombatModel.h>
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

    EntityCombatModel& combat();
    const EntityCombatModel& combat() const;

private:
    EntityPositionModel _position;
    EntitySizeModel _size;
    EntityVitalsModel _vitals;
    EntityMovementModel _movement;
    EntityCombatModel _combat;
};

} // namespace Engine

#endif // ENTITYMODEL_H
