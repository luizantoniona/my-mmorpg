#include "CreatureTypeCatalog.h"

namespace Engine {

CreatureTypeCatalog::CreatureTypeCatalog() :
    _creatureTypes() {
}

const CreatureTypeModel* CreatureTypeCatalog::creatureType( uint32_t type ) const {
    auto iterator = _creatureTypes.find( type );

    if ( iterator == _creatureTypes.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::unordered_map<uint32_t, CreatureTypeModel>& CreatureTypeCatalog::creatureTypes() const {
    return _creatureTypes;
}

void CreatureTypeCatalog::addCreatureType( const CreatureTypeModel& creatureType ) {
    _creatureTypes.insert( { creatureType.type(), creatureType } );
}

} // namespace Engine
