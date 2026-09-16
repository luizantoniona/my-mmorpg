#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <MMORPGEngine/Entity/EntityOrientationModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/EntitySizeModel.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>

namespace Engine {

class EntityModel {
public:
    EntityModel();
    ~EntityModel();

    EntityOrientationModel orientation() const;
    void setOrientation( const EntityOrientationModel& orientation );

    EntityPositionModel position() const;
    void setPosition( const EntityPositionModel& position );

    EntitySizeModel size() const;
    void setSize( const EntitySizeModel& size );

    EntityVitalsModel vitals() const;
    void setVitals( const EntityVitalsModel& vitals );

private:
    EntityOrientationModel _orientation;
    EntityPositionModel _position;
    EntitySizeModel _size;
    EntityVitalsModel _vitals;
};

} // namespace Engine

#endif // ENTITYMODEL_H
