#include "SkillNodeModel.h"

namespace Engine {

SkillNodeModel::SkillNodeModel() :
    _name( "" ),
    _prerequisites(),
    _type( 0 ),
    _proficiencyLevel( 0 ) {
}

uint32_t SkillNodeModel::type() const {
    return _type;
}

void SkillNodeModel::setType( uint32_t type ) {
    _type = type;
}

QString SkillNodeModel::name() const {
    return _name;
}

void SkillNodeModel::setName( const QString& name ) {
    _name = name;
}

uint32_t SkillNodeModel::proficiencyLevel() const {
    return _proficiencyLevel;
}

void SkillNodeModel::setProficiencyLevel( uint32_t proficiencyLevel ) {
    _proficiencyLevel = proficiencyLevel;
}

QList<uint32_t> SkillNodeModel::prerequisites() const {
    return _prerequisites;
}

void SkillNodeModel::setPrerequisites( const QList<uint32_t>& prerequisites ) {
    _prerequisites = prerequisites;
}

} // namespace Engine
