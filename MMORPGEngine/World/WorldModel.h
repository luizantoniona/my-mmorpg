#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include <cstdint>
#include <map>
#include <memory>

#include <QString>

#include <MMORPGEngine/World/Chunk/ChunkModel.h>
#include <MMORPGEngine/World/Tile/TileModel.h>

namespace Engine {

class WorldModel {
public:
    WorldModel();

    QString name() const;
    void setName( const QString& name );

    uint32_t width() const;
    void setWidth( uint32_t width );

    uint32_t height() const;
    void setHeight( uint32_t height );

    ChunkModel* chunk( int x, int y );
    const ChunkModel* chunk( int x, int y ) const;

    const TileModel* tile( int x, int y, int z ) const;

private:
    QString chunkKey( int x, int y ) const;

private:
    QString _name;
    uint32_t _width;
    uint32_t _height;
    std::map<QString, std::unique_ptr<ChunkModel>> _chunks;
};

} // namespace Engine

#endif // WORLDMODEL_H
