#ifndef GROUNDCATALOG_H
#define GROUNDCATALOG_H

#include <unordered_map>

#include <MMORPGEngine/Core/World/Model/GroundModel.h>

namespace Engine {

class GroundCatalog {
public:
    GroundCatalog();

    const GroundModel* ground( uint16_t type ) const;
    const std::unordered_map<uint16_t, GroundModel>& grounds() const;
    void addGround( const GroundModel& ground );

private:
    std::unordered_map<uint16_t, GroundModel> _grounds;
};

} // namespace Engine

#endif // GROUNDCATALOG_H
