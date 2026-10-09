#include "EntitySizeModel.h"

namespace Engine {

EntitySizeModel::EntitySizeModel() :
    _width( 1 ),
    _height( 1 ) {
}

EntitySizeModel::~EntitySizeModel() {
}

int EntitySizeModel::width() const {
    return _width;
}

void EntitySizeModel::setWidth( int width ) {
    _width = width;
}

int EntitySizeModel::height() const {
    return _height;
}

void EntitySizeModel::setHeight( int height ) {
    _height = height;
}

} // namespace Engine
