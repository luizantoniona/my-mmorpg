#include "GroundCatalog.h"

namespace Engine {

GroundCatalog::GroundCatalog() :
    _grounds() {}

const GroundModel* GroundCatalog::ground( uint16_t type ) const {
    auto iterator = _grounds.find( type );

    if ( iterator == _grounds.end() ) {
        return nullptr;
    }

    return &iterator.value();
}

const QMap<uint16_t, GroundModel>& GroundCatalog::grounds() const {
    return _grounds;
}

void GroundCatalog::addGround( const GroundModel& ground ) {
    _grounds.insert( ground.type(), ground );
}

} // namespace Engine
