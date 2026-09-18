#include "ItemTypeModel.h"

namespace Engine {

ItemTypeModel::ItemTypeModel() :
    _name( "" ),
    _type( 0 ) {
}

uint32_t ItemTypeModel::type() const {
    return _type;
}

void ItemTypeModel::setType( uint32_t type ) {
    _type = type;
}

QString ItemTypeModel::name() const {
    return _name;
}

void ItemTypeModel::setName( const QString& name ) {
    _name = name;
}

} // namespace Engine
