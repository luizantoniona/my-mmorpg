#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include <QString>

#include <MMORPGEngine/World/Chunk/ChunkModel.h>
#include <MMORPGEngine/World/Tile/WorldTileModel.h>

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

    int spawnX() const;
    void setSpawnX( int spawnX );

    int spawnY() const;
    void setSpawnY( int spawnY );

    int spawnZ() const;
    void setSpawnZ( int spawnZ );

    ChunkModel* chunk( int x, int y );
    const ChunkModel* chunk( int x, int y ) const;

    std::vector<int> floors() const;
    void addFloor( int z );

    const WorldObjectModel* object( int x, int y, int z ) const;
    void setObject( int x, int y, int z, uint32_t objectType );

    const WorldTileModel* tile( int x, int y, int z ) const;
    void setTile( int x, int y, int z, uint32_t tileType );

private:
    QString chunkKey( int x, int y ) const;

private:
    QString _name;
    uint32_t _width;
    uint32_t _height;
    int _spawnX;
    int _spawnY;
    int _spawnZ;
    std::map<QString, std::unique_ptr<ChunkModel>> _chunks;
    std::set<int> _floors;
};

} // namespace Engine

#endif // WORLDMODEL_H
