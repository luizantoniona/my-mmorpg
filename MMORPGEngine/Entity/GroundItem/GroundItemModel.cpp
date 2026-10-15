#include "GroundItemModel.h"

namespace Engine {

GroundItemModel::GroundItemModel() :
    _idItem( "" ),
    _idGroundItem( 0 ) {
}

int GroundItemModel::idGroundItem() const {
    return _idGroundItem;
}

void GroundItemModel::setIdGroundItem( int idGroundItem ) {
    _idGroundItem = idGroundItem;
}

QString GroundItemModel::idItem() const {
    return _idItem;
}

void GroundItemModel::setIdItem( const QString& idItem ) {
    _idItem = idItem;
}

const EntityPositionModel& GroundItemModel::position() const {
    return _position;
}

EntityPositionModel& GroundItemModel::position() {
    return _position;
}

} // namespace Engine
