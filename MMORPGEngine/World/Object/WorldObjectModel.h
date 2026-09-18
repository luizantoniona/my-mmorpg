#ifndef WORLDOBJECTMODEL_H
#define WORLDOBJECTMODEL_H

#include <cstdint>

#include <MMORPGEngine/Data/Object/ObjectModel.h>

namespace Engine {

class WorldObjectModel {
public:
    WorldObjectModel();

    const ObjectModel* objectModel() const;
    void setObjectModel( const ObjectModel* objectModel );

    uint32_t objectType() const;
    void setObjectType( uint32_t objectType );

    int x() const;
    void setX( int x );

    int y() const;
    void setY( int y );

    int z() const;
    void setZ( int z );

private:
    const ObjectModel* _objectModel;
    uint32_t _objectType;
    int _x;
    int _y;
    int _z;
};

} // namespace Engine

#endif // WORLDOBJECTMODEL_H
