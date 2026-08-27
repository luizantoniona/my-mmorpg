#ifndef CHUNKMODEL_H
#define CHUNKMODEL_H

#include <map>
#include <memory>
#include <vector>

#include <MMORPGEngine/World/Object/WorldObjectModel.h>
#include <MMORPGEngine/World/Tile/WorldTileModel.h>

namespace Engine {

class ChunkModel {
public:
    ChunkModel();
    ~ChunkModel();

    static constexpr int CHUNK_WIDTH = 32;
    static constexpr int CHUNK_HEIGHT = 32;

    void setObject( int x, int y, int z, std::unique_ptr<WorldObjectModel> object );
    WorldObjectModel* object( int x, int y, int z );
    const WorldObjectModel* object( int x, int y, int z ) const;

    void setTile( int x, int y, int z, std::unique_ptr<WorldTileModel> tile );
    WorldTileModel* tile( int x, int y, int z );
    const WorldTileModel* tile( int x, int y, int z ) const;

private:
    bool validPosition( int x, int y ) const;
    int index( int x, int y ) const;

private:
    std::map<int, std::vector<std::unique_ptr<WorldObjectModel>>> _objects;
    std::map<int, std::vector<std::unique_ptr<WorldTileModel>>> _tiles;
};

} // namespace Engine

#endif // CHUNKMODEL_H
