#include "CreatureTypeCatalog.h"

namespace Engine {

CreatureTypeCatalog::CreatureTypeCatalog() :
    _creatureTypes() {
}

const CreatureTypeModel* CreatureTypeCatalog::creatureType( const QString& type ) const {
    auto iterator = _creatureTypes.find( type );

    if ( iterator == _creatureTypes.end() ) {
        return nullptr;
    }

    return &iterator->second;
}

const std::map<QString, CreatureTypeModel>& CreatureTypeCatalog::creatureTypes() const {
    return _creatureTypes;
}

void CreatureTypeCatalog::addCreatureType( const CreatureTypeModel& creatureType ) {
    _creatureTypes.insert( { creatureType.type(), creatureType } );
}

} // namespace Engine
