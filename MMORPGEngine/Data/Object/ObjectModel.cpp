#include "ObjectModel.h"

namespace Engine {

ObjectModel::ObjectModel() :
    _type( 0 ),
    _name( "" ),
    _textureFile( "" ),
    _animation(),
    _size(),
    _interaction( std::nullopt ),
    _isWalkable( true ) {
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

QString ObjectModel::textureFile() const {
    return _textureFile;
}

void ObjectModel::setTextureFile( const QString& textureFile ) {
    _textureFile = textureFile;
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

const std::optional<ObjectInteractionModel>& ObjectModel::interaction() const {
    return _interaction;
}

void ObjectModel::setInteraction( const ObjectInteractionModel& interaction ) {
    _interaction = interaction;
}

bool ObjectModel::isWalkable() const {
    return _isWalkable;
}

void ObjectModel::setIsWalkable( bool isWalkable ) {
    _isWalkable = isWalkable;
}

} // namespace Engine
