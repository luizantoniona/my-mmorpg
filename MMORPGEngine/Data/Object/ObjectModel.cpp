#include "ObjectModel.h"

namespace Engine {

ObjectModel::ObjectModel() :
    _type( 0 ),
    _name( "" ),
    _folder( "" ),
    _texture(),
    _size() {
}

ObjectModel::~ObjectModel() = default;

uint32_t ObjectModel::type() const {
    return _type;
}

void ObjectModel::setType( uint32_t type ) {
    _type = type;
}

QString ObjectModel::name() const {
    return _name;
}

void ObjectModel::setName( const QString& name ) {
    _name = name;
}

QString ObjectModel::folder() const {
    return _folder;
}

void ObjectModel::setFolder( const QString& folder ) {
    _folder = folder;
}

QImage ObjectModel::texture() const {
    return _texture;
}

void ObjectModel::setTexture( const QImage& texture ) {
    _texture = texture;
}

ObjectSizeModel ObjectModel::size() const {
    return _size;
}

void ObjectModel::setSize( const ObjectSizeModel& size ) {
    _size = size;
}

} // namespace Engine
