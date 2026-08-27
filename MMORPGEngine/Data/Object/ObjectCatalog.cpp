#include "ObjectCatalog.h"

namespace Engine {

ObjectCatalog::ObjectCatalog() :
    _objects() {
}

const ObjectModel* ObjectCatalog::texture( uint32_t type ) const {
    auto iterator = _objects.find( type );

    if ( iterator == _objects.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::unordered_map<uint32_t, ObjectModel>& ObjectCatalog::textures() const {
    return _objects;
}

void ObjectCatalog::addTexture( const ObjectModel& object ) {
    _objects.insert( { object.type(), object } );
}

} // namespace Engine
