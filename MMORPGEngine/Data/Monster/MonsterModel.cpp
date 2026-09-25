#include "MonsterModel.h"

namespace Engine {

MonsterModel::MonsterModel() :
    _name( "" ),
    _type( 0 ) {
}

uint32_t MonsterModel::type() const {
    return _type;
}

void MonsterModel::setType( uint32_t type ) {
    _type = type;
}

QString MonsterModel::name() const {
    return _name;
}

void MonsterModel::setName( const QString& name ) {
    _name = name;
}

} // namespace Engine
