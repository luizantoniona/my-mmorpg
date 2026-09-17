#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/EntitySizeModel.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>

namespace Engine {

class EntityModel {
public:
    EntityModel();
    ~EntityModel();

    EntityPositionModel position() const;
    void setPosition( const EntityPositionModel& position );

    EntitySizeModel size() const;
    void setSize( const EntitySizeModel& size );

    EntityVitalsModel vitals() const;
    void setVitals( const EntityVitalsModel& vitals );

private:
    EntityPositionModel _position;
    EntitySizeModel _size;
    EntityVitalsModel _vitals;
};

} // namespace Engine

#endif // ENTITYMODEL_H
