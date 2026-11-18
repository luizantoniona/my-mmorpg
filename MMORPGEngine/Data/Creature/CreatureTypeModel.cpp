#include "CreatureTypeModel.h"

namespace Engine {

CreatureTypeModel::CreatureTypeModel() :
    _type( "" ),
    _name( "" ),
    _vitals(),
    _appearance(),
    _attack( std::nullopt ),
    _damage( 0.0 ),
    _aggroRadius( 0 ),
    _attackRange( 1 ),
    _footprint( 1 ) {
}

QString CreatureTypeModel::type() const {
    return _type;
}

void CreatureTypeModel::setType( const QString& type ) {
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

uint32_t CreatureTypeModel::attackRange() const {
    return _attackRange;
}

void CreatureTypeModel::setAttackRange( uint32_t attackRange ) {
    _attackRange = attackRange;
}

uint32_t CreatureTypeModel::footprint() const {
    return _footprint;
}

void CreatureTypeModel::setFootprint( uint32_t footprint ) {
    _footprint = footprint;
}

double CreatureTypeModel::damage() const {
    return _damage;
}

void CreatureTypeModel::setDamage( double damage ) {
    _damage = damage;
}

const std::optional<AttackModel>& CreatureTypeModel::attack() const {
    return _attack;
}

void CreatureTypeModel::setAttack( const AttackModel& attack ) {
    _attack = attack;
}

CreatureTypeVitalsModel& CreatureTypeModel::vitals() {
    return _vitals;
}

const CreatureTypeVitalsModel& CreatureTypeModel::vitals() const {
    return _vitals;
}

const AppearanceModel& CreatureTypeModel::appearance() const {
    return _appearance;
}

void CreatureTypeModel::setAppearance( const AppearanceModel& appearance ) {
    _appearance = appearance;
}

} // namespace Engine
