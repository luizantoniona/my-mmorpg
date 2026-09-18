#include "ObjectModel.h"

namespace Engine {

ObjectModel::ObjectModel() :
    _type( 0 ),
    _name( "" ),
    _folder( "" ),
    _animation(),
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
    return _animation.firstFrame();
}

void ObjectModel::setTexture( const QImage& texture ) {
    _animation = AnimationModel( { texture }, 100 );
}

AnimationModel ObjectModel::animation() const {
    return _animation;
}

void ObjectModel::setAnimation( const AnimationModel& animation ) {
    _animation = animation;
}

ObjectSizeModel ObjectModel::size() const {
    return _size;
}

void ObjectModel::setSize( const ObjectSizeModel& size ) {
    _size = size;
}

} // namespace Engine
