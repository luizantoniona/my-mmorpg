#include "ObjectSizeModel.h"

namespace Engine {

ObjectSizeModel::ObjectSizeModel() :
    _width( 0 ),
    _height( 0 ) {
}

ObjectSizeModel::~ObjectSizeModel() {
}

int ObjectSizeModel::width() const {
    return _width;
}

void ObjectSizeModel::setWidth( int width ) {
    _width = width;
}

int ObjectSizeModel::height() const {
    return _height;
}

void ObjectSizeModel::setHeight( int height ) {
    _height = height;
}

} // namespace Engine
