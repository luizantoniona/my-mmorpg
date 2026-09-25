#ifndef OBJECTINTERACTIONMODEL_H
#define OBJECTINTERACTIONMODEL_H

#include <cstdint>

#include <MMORPGEngine/Data/Object/ObjectInteractionEnum.h>

namespace Engine {

class ObjectInteractionModel {
public:
    ObjectInteractionModel();

    ObjectInteractionEnum type() const;
    void setType( ObjectInteractionEnum type );

    uint32_t containerCapacity() const;
    void setContainerCapacity( uint32_t containerCapacity );

private:
    ObjectInteractionEnum _type;
    uint32_t _containerCapacity;
};

} // namespace Engine

#endif // OBJECTINTERACTIONMODEL_H
