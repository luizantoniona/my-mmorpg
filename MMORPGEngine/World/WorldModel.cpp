#include "WorldModel.h"

namespace {
constexpr int CHUNK_WIDTH = 32;
constexpr int CHUNK_HEIGHT = 32;
} // namespace

namespace Engine {

WorldModel::WorldModel() :
    _name( "" ),
    _width( 0 ),
    _height( 0 ) {
}

QString WorldModel::name() const {
    return _name;
}

void WorldModel::setName( const QString& name ) {
    _name = name;
}

uint32_t WorldModel::width() const {
    return _width;
}

void WorldModel::setWidth( uint32_t width ) {
    _width = width;
}

uint32_t WorldModel::height() const {
    return _height;
}

void WorldModel::setHeight( uint32_t height ) {
    _height = height;
}

ChunkModel* WorldModel::chunk( int x, int y ) {
    const QString key = chunkKey( x, y );

    auto iterator = _chunks.find( key );

    if ( iterator == _chunks.end() ) {
        iterator = _chunks.insert( key, std::make_unique<ChunkModel>() );
    }

    return iterator.value().get();
}

const ChunkModel* WorldModel::chunk( int x, int y ) const {
    const auto iterator = _chunks.constFind( chunkKey( x, y ) );

    if ( iterator == _chunks.constEnd() ) {
        return nullptr;
    }

    return iterator.value().get();
}

const TileModel* WorldModel::tile( int x, int y, int z ) const {
    const int chunkX = x / CHUNK_WIDTH;
    const int chunkY = y / CHUNK_HEIGHT;

    const int localX = x % CHUNK_WIDTH;
    const int localY = y % CHUNK_HEIGHT;

    const auto iterator = _chunks.constFind( chunkKey( chunkX, chunkY ) );

    if ( iterator == _chunks.constEnd() ) {
        return nullptr;
    }

    return iterator.value()->tile( localX, localY, z );
}

QString WorldModel::chunkKey( int x, int y ) const {
    return QStringLiteral( "%1:%2" ).arg( x ).arg( y );
}

} // namespace Engine
