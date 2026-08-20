#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <cstdint>

#include <MMORPGEngine/Object/ObjectOrientationModel.h>
#include <MMORPGEngine/Object/ObjectPositionModel.h>
#include <MMORPGEngine/Object/ObjectSizeModel.h>

namespace Engine {

class ObjectModel {
public:
    ObjectModel();
    ~ObjectModel();

    uint32_t objectTextureId() const;
    void setObjectTextureId( uint32_t objectTextureId );

    ObjectOrientationModel orientation() const;
    void setOrientation( const ObjectOrientationModel& orientation );

    ObjectPositionModel position() const;
    void setPosition( const ObjectPositionModel& position );

    ObjectSizeModel size() const;
    void setSize( const ObjectSizeModel& size );

private:
    uint32_t _objectTextureId;
    ObjectOrientationModel _orientation;
    ObjectPositionModel _position;
    ObjectSizeModel _size;
};

} // namespace Engine

#endif // OBJECTMODEL_H
