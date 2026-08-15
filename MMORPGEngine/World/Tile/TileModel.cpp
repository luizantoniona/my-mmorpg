#include "TileModel.h"

namespace Engine {

TileModel::TileModel() :
    _groundId( 0 ) {
}

uint16_t TileModel::groundId() const {
    return _groundId;
}

void TileModel::setGroundId( uint16_t groundId ) {
    _groundId = groundId;
}

} // namespace Engine
