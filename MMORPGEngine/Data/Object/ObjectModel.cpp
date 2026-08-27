#include "ObjectModel.h"

namespace Engine {

ObjectModel::ObjectModel() :
    _type( 0 ),
    _name( "" ),
    _folder( "" ),
    _texture(),
    _orientation(),
    _position(),
    _size() {
}

ObjectModel::~ObjectModel() = default;

uint32_t ObjectTextureModel::type() const {
    return _type;
}

void ObjectTextureModel::setType( uint32_t type ) {
    _type = type;
}

QString ObjectTextureModel::name() const {
    return _name;
}

void ObjectTextureModel::setName( const QString& name ) {
    _name = name;
}

QString ObjectTextureModel::folder() const {
    return _folder;
}

void ObjectTextureModel::setFolder( const QString& folder ) {
    _folder = folder;
}

QImage ObjectTextureModel::texture() const {
    return _texture;
}

void ObjectTextureModel::setTexture( const QImage& texture ) {
    _texture = texture;
}

ObjectOrientationModel ObjectModel::orientation() const {
    return _orientation;
}

void ObjectModel::setOrientation( const ObjectOrientationModel& orientation ) {
    _orientation = orientation;
}

ObjectPositionModel ObjectModel::position() const {
    return _position;
}

void ObjectModel::setPosition( const ObjectPositionModel& position ) {
    _position = position;
}

ObjectSizeModel ObjectModel::size() const {
    return _size;
}

void ObjectModel::setSize( const ObjectSizeModel& size ) {
    _size = size;
}

} // namespace Engine
