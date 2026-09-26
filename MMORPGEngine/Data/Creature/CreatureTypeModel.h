#ifndef CREATURETYPEMODEL_H
#define CREATURETYPEMODEL_H

#include <cstdint>

#include <QString>

#include <MMORPGEngine/Data/Creature/CreatureTypeVitalsModel.h>

namespace Engine {

class CreatureTypeModel {
public:
    CreatureTypeModel();

    uint32_t type() const;
    void setType( uint32_t type );

    QString name() const;
    void setName( const QString& name );

    CreatureTypeVitalsModel& vitals();
    const CreatureTypeVitalsModel& vitals() const;

private:
    QString _name;
    CreatureTypeVitalsModel _vitals;
    uint32_t _type;
};

} // namespace Engine

#endif // CREATURETYPEMODEL_H
