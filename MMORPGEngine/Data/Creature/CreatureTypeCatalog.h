#ifndef CREATURETYPECATALOG_H
#define CREATURETYPECATALOG_H

#include <map>

#include <QString>

#include <MMORPGEngine/Data/Creature/CreatureTypeModel.h>

namespace Engine {

class CreatureTypeCatalog {
public:
    CreatureTypeCatalog();

    const CreatureTypeModel* creatureType( const QString& type ) const;
    const std::map<QString, CreatureTypeModel>& creatureTypes() const;
    void addCreatureType( const CreatureTypeModel& creatureType );

private:
    std::map<QString, CreatureTypeModel> _creatureTypes;
};

} // namespace Engine

#endif // CREATURETYPECATALOG_H
