#include "TileModel.h"

namespace Engine {

TileModel::TileModel() :
    _type( 0 ),
    _name( "" ),
    _folder( "" ),
    _texture() {
}

uint32_t TileModel::type() const {
    return _type;
}

void TileModel::setType( uint32_t type ) {
    _type = type;
}

QString TileModel::name() const {
    return _name;
}

void TileModel::setName( const QString& name ) {
    _name = name;
}

QString TileModel::folder() const {
    return _folder;
}

void TileModel::setFolder( const QString& folder ) {
    _folder = folder;
}

QImage TileModel::texture() const {
    return _texture;
}

void TileModel::setTexture( const QImage& texture ) {
    _texture = texture;
}

} // namespace Engine
