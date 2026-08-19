#ifndef CHUNKMODEL_H
#define CHUNKMODEL_H

#include <map>
#include <memory>
#include <vector>

#include <MMORPGEngine/World/Tile/TileModel.h>

namespace Engine {

class ChunkModel {
public:
    ChunkModel();

    static constexpr int CHUNK_WIDTH = 32;
    static constexpr int CHUNK_HEIGHT = 32;

    void setTile( int x, int y, int z, std::unique_ptr<TileModel> tile );
    TileModel* tile( int x, int y, int z );
    const TileModel* tile( int x, int y, int z ) const;

private:
    std::map<int, std::vector<std::unique_ptr<TileModel>>> _tiles;
};

} // namespace Engine

#endif // CHUNKMODEL_H
