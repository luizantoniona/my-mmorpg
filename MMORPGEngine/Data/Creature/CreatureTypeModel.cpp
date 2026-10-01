#include "CreatureTypeModel.h"

namespace Engine {

CreatureTypeModel::CreatureTypeModel() :
    _name( "" ),
    _type( 0 ),
    _aggroRadius( 0 ) {
}

uint32_t CreatureTypeModel::type() const {
    return _type;
}

void CreatureTypeModel::setType( uint32_t type ) {
    _type = type;
}

QString CreatureTypeModel::name() const {
    return _name;
}

void CreatureTypeModel::setName( const QString& name ) {
    _name = name;
}

uint32_t CreatureTypeModel::aggroRadius() const {
    return _aggroRadius;
}

void CreatureTypeModel::setAggroRadius( uint32_t aggroRadius ) {
    _aggroRadius = aggroRadius;
}

CreatureTypeVitalsModel& CreatureTypeModel::vitals() {
    return _vitals;
}

const CreatureTypeVitalsModel& CreatureTypeModel::vitals() const {
    return _vitals;
}

} // namespace Engine
