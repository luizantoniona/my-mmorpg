#include "ItemModel.h"

namespace Engine {

ItemModel::ItemModel() :
    _name( "" ),
    _id( 0 ),
    _idItemType( 0 ) {
}

uint32_t ItemModel::id() const {
    return _id;
}

void ItemModel::setId( uint32_t id ) {
    _id = id;
}

uint32_t ItemModel::idItemType() const {
    return _idItemType;
}

void ItemModel::setIdItemType( uint32_t idItemType ) {
    _idItemType = idItemType;
}

QString ItemModel::name() const {
    return _name;
}

void ItemModel::setName( const QString& name ) {
    _name = name;
}

} // namespace Engine
