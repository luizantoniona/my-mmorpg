#include "TileModel.h"

namespace Engine {

TileModel::TileModel() :
    _name( "" ),
    _folder( "" ),
    _animation(),
    _tags(),
    _type( 0 ),
    _isWalkable( true ) {
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
    return _animation.firstFrame();
}

void TileModel::setTexture( const QImage& texture ) {
    _animation = AnimationModel( { texture }, 100 );
}

AnimationModel TileModel::animation() const {
    return _animation;
}

void TileModel::setAnimation( const AnimationModel& animation ) {
    _animation = animation;
}

QList<QString> TileModel::tags() const {
    return _tags;
}

void TileModel::setTags( const QList<QString>& tags ) {
    _tags = tags;
}

bool TileModel::isWalkable() const {
    return _isWalkable;
}

void TileModel::setIsWalkable( bool isWalkable ) {
    _isWalkable = isWalkable;
}

} // namespace Engine
