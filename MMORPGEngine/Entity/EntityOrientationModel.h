#ifndef ENTITYORIENTATIONMODEL_H
#define ENTITYORIENTATIONMODEL_H

#include <string>

#include <MMORPGEngine/Entity/EntityOrientationEnum.h>

namespace Engine {

class EntityOrientationModel {
public:
    EntityOrientationModel();
    ~EntityOrientationModel();

    static EntityOrientationEnum fromMovement( int dx, int dy );
    static std::string toString( EntityOrientationEnum orientation );
    static EntityOrientationEnum fromString( const std::string& value );

    EntityOrientationEnum direction() const;
    void setDirection( EntityOrientationEnum direction );

private:
    EntityOrientationEnum _direction;
};

} // namespace Engine

#endif // ENTITYORIENTATIONMODEL_H
