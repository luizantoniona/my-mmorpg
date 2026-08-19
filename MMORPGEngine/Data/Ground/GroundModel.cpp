#include "GroundModel.h"

namespace Engine {

GroundModel::GroundModel() :
    _type( 0 ),
    _name( "" ),
    _folder( "" ) {
}

uint32_t GroundModel::type() const {
    return _type;
}

void GroundModel::setType( uint32_t type ) {
    _type = type;
}

QString GroundModel::name() const {
    return _name;
}

void GroundModel::setName( const QString& name ) {
    _name = name;
}

QString GroundModel::folder() const {
    return _folder;
}

void GroundModel::setFolder( const QString& folder ) {
    _folder = folder;
}

} // namespace Engine
