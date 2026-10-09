#ifndef CREATURESPAWNENTRYMODEL_H
#define CREATURESPAWNENTRYMODEL_H

#include <cstdint>

#include <QString>

namespace Engine {

class CreatureSpawnEntryModel {
public:
    CreatureSpawnEntryModel();

    QString type() const;
    void setType( const QString& type );

    uint32_t quantity() const;
    void setQuantity( uint32_t quantity );

private:
    QString _type;
    uint32_t _quantity;
};

} // namespace Engine

#endif // CREATURESPAWNENTRYMODEL_H
