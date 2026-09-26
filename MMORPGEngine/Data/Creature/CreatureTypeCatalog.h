#ifndef CREATURETYPECATALOG_H
#define CREATURETYPECATALOG_H

#include <unordered_map>

#include <MMORPGEngine/Data/Creature/CreatureTypeModel.h>

namespace Engine {

class CreatureTypeCatalog {
public:
    CreatureTypeCatalog();

    const CreatureTypeModel* creatureType( uint32_t type ) const;
    const std::unordered_map<uint32_t, CreatureTypeModel>& creatureTypes() const;
    void addCreatureType( const CreatureTypeModel& creatureType );

private:
    std::unordered_map<uint32_t, CreatureTypeModel> _creatureTypes;
};

} // namespace Engine

#endif // CREATURETYPECATALOG_H
