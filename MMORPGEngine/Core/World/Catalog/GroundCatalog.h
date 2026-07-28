#ifndef GROUNDCATALOG_H
#define GROUNDCATALOG_H

#include <QMap>

#include <MMORPGEngine/Core/World/Model/GroundModel.h>

namespace Engine {

class GroundCatalog {
public:
    GroundCatalog();

    const GroundModel* ground( uint16_t type ) const;
    const QMap<uint16_t, GroundModel>& grounds() const;
    void addGround( const GroundModel& ground );

private:
    QMap<uint16_t, GroundModel> _grounds;
};

} // namespace Engine

#endif // GROUNDCATALOG_H
