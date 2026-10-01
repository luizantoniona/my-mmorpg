#include "EntityAnimationModel.h"

#include <algorithm>
#include <cstdlib>

#include <QDateTime>

#include <MMORPGEngine/World/WorldConstants.h>

namespace Engine {

namespace {

constexpr int MILLISECONDS_PER_SECOND = 1000;

} // namespace

EntityAnimationModel::EntityAnimationModel() :
    EntityAnimationModel( 0, 0, 0, 0.0 ) {
}

EntityAnimationModel::EntityAnimationModel( int x, int y, int z, double movementSeconds ) :
    _startMs( 0 ),
    _durationMs( static_cast<int>( movementSeconds * MILLISECONDS_PER_SECOND ) ),
    _x( x ),
    _y( y ),
    _z( z ),
    _previousX( x ),
    _previousY( y ) {
}

int EntityAnimationModel::x() const {
    return _x;
}

int EntityAnimationModel::y() const {
    return _y;
}

int EntityAnimationModel::z() const {
    return _z;
}

void EntityAnimationModel::moveTo( int x, int y, int z, double movementSeconds ) {
    _durationMs = static_cast<int>( movementSeconds * MILLISECONDS_PER_SECOND );

    const bool changedFloor = z != _z;
    const bool changedTile = x != _x || y != _y;

    if ( !changedTile && !changedFloor ) {
        return;
    }

    const bool isNeighbourTile = std::abs( x - _x ) <= 1 && std::abs( y - _y ) <= 1 && !changedFloor;

    _previousX = _x;
    _previousY = _y;
    _x = x;
    _y = y;
    _z = z;

    if ( !isNeighbourTile || !changedTile || _durationMs <= 0 ) {
        snap();
        return;
    }

    _startMs = QDateTime::currentMSecsSinceEpoch();
}

double EntityAnimationModel::offsetX() const {
    return ( _previousX - _x ) * WorldConstants::TILE_SIZE * remainingRatio();
}

double EntityAnimationModel::offsetY() const {
    return ( _previousY - _y ) * WorldConstants::TILE_SIZE * remainingRatio();
}

double EntityAnimationModel::remainingRatio() const {
    if ( _durationMs <= 0 ) {
        return 0.0;
    }

    const qint64 elapsedMs = QDateTime::currentMSecsSinceEpoch() - _startMs;

    return std::clamp( 1.0 - static_cast<double>( elapsedMs ) / _durationMs, 0.0, 1.0 );
}

void EntityAnimationModel::snap() {
    _previousX = _x;
    _previousY = _y;
    _startMs = 0;
}

} // namespace Engine
