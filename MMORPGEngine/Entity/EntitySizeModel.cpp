#include "EntitySizeModel.h"

namespace Engine {

EntitySizeModel::EntitySizeModel() :
    _width( 0 ),
    _height( 0 ) {
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
