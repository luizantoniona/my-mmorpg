#include "GroundModel.h"

namespace Engine {

GroundModel::GroundModel() :
    _type( 0 ),
    _walkable( false ),
    _name( "" ),
    _folder( "" ) {}

uint16_t GroundModel::type() const {
    return _type;
}

void GroundModel::setType( uint16_t type ) {
    _type = type;
}

bool GroundModel::walkable() const {
    return _walkable;
}

void GroundModel::setWalkable( bool walkable ) {
    _walkable = walkable;
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
