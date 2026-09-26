#ifndef CREATURESPAWNENTRYMODEL_H
#define CREATURESPAWNENTRYMODEL_H

#include <cstdint>

namespace Engine {

class CreatureSpawnEntryModel {
public:
    CreatureSpawnEntryModel();

    uint32_t type() const;
    void setType( uint32_t type );

    uint32_t quantity() const;
    void setQuantity( uint32_t quantity );

private:
    uint32_t _type;
    uint32_t _quantity;
};

} // namespace Engine

#endif // CREATURESPAWNENTRYMODEL_H
