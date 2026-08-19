#ifndef CHUNKMODEL_H
#define CHUNKMODEL_H

#include <memory>
#include <vector>

#include <QMap>

#include <MMORPGEngine/World/Tile/TileModel.h>

namespace Engine {

class ChunkModel {
public:
    ChunkModel();

    void setTile( int x, int y, int z, std::unique_ptr<TileModel> tile );
    const TileModel* tile( int x, int y, int z ) const;

private:
    QMap<int, std::vector<std::unique_ptr<TileModel>>> _tiles;
};

} // namespace Engine

#endif // CHUNKMODEL_H
