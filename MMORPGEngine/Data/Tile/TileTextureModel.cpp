#include "TileTextureModel.h"

namespace Engine {

TileTextureModel::TileTextureModel() :
    _type( 0 ),
    _name( "" ),
    _folder( "" ),
    _texture() {
}

uint32_t TileTextureModel::type() const {
    return _type;
}

void TileTextureModel::setType( uint32_t type ) {
    _type = type;
}

QString TileTextureModel::name() const {
    return _name;
}

void TileTextureModel::setName( const QString& name ) {
    _name = name;
}

QString TileTextureModel::folder() const {
    return _folder;
}

void TileTextureModel::setFolder( const QString& folder ) {
    _folder = folder;
}

QImage TileTextureModel::texture() const {
    return _texture;
}

void TileTextureModel::setTexture( const QImage& texture ) {
    _texture = texture;
}

} // namespace Engine
