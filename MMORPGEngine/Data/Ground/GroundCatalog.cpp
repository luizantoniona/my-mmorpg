#include "GroundCatalog.h"

namespace Engine {

GroundCatalog::GroundCatalog() :
    _grounds() {
}

const GroundModel* GroundCatalog::ground( uint32_t type ) const {
    auto iterator = _grounds.find( type );

    if ( iterator == _grounds.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::unordered_map<uint32_t, GroundModel>& GroundCatalog::grounds() const {
    return _grounds;
}

void GroundCatalog::addGround( const GroundModel& ground ) {
    _grounds.insert( { ground.type(), ground } );
}

} // namespace Engine
