#include "ObjectTextureModel.h"

namespace Engine {

ObjectTextureModel::ObjectTextureModel() :
    _type( 0 ),
    _name( "" ),
    _folder( "" ),
    _texture() {
}

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

} // namespace Engine
