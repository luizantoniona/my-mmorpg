#include "ObjectCatalog.h"

#include <algorithm>

namespace Engine {

ObjectCatalog::ObjectCatalog() :
    _objects(),
    _maxBlockingFootprintWidth( 0 ),
    _maxBlockingFootprintHeight( 0 ) {
}

const ObjectModel* ObjectCatalog::object( uint32_t type ) const {
    auto iterator = _objects.find( type );

    if ( iterator == _objects.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::unordered_map<uint32_t, ObjectModel>& ObjectCatalog::objects() const {
    return _objects;
}

void ObjectCatalog::addObject( const ObjectModel& object ) {
    _objects.insert( { object.type(), object } );
    recomputeMaxBlockingFootprint();
}

void ObjectCatalog::replaceObject( const ObjectModel& object ) {
    _objects[ object.type() ] = object;
    recomputeMaxBlockingFootprint();
}

int ObjectCatalog::maxBlockingFootprintWidth() const {
    return _maxBlockingFootprintWidth;
}

int ObjectCatalog::maxBlockingFootprintHeight() const {
    return _maxBlockingFootprintHeight;
}

void ObjectCatalog::recomputeMaxBlockingFootprint() {
    _maxBlockingFootprintWidth = 0;
    _maxBlockingFootprintHeight = 0;

    for ( const auto& entry : _objects ) {
        const ObjectModel& object = entry.second;

        if ( object.isWalkable() ) {
            continue;
        }

        _maxBlockingFootprintWidth = std::max( _maxBlockingFootprintWidth, std::max( object.size().width(), 1 ) );
        _maxBlockingFootprintHeight = std::max( _maxBlockingFootprintHeight, std::max( object.size().height(), 1 ) );
    }
}

} // namespace Engine
