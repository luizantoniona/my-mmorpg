#include "ItemModel.h"

namespace Engine {

ItemModel::ItemModel() :
    _id( "" ),
    _name( "" ),
    _idItemType( "" ),
    _textureFile( "" ),
    _animation() {
}

QString ItemModel::id() const {
    return _id;
}

void ItemModel::setId( const QString& id ) {
    _id = id;
}

QString ItemModel::idItemType() const {
    return _idItemType;
}

void ItemModel::setIdItemType( const QString& idItemType ) {
    _idItemType = idItemType;
}

QString ItemModel::name() const {
    return _name;
}

void ItemModel::setName( const QString& name ) {
    _name = name;
}

QString ItemModel::textureFile() const {
    return _textureFile;
}

void ItemModel::setTextureFile( const QString& textureFile ) {
    _textureFile = textureFile;
}

QImage ItemModel::texture() const {
    return _animation.firstFrame();
}

AnimationModel ItemModel::animation() const {
    return _animation;
}

void ItemModel::setAnimation( const AnimationModel& animation ) {
    _animation = animation;
}

} // namespace Engine
