#ifndef WORLDVISIBILITYDELTA_H
#define WORLDVISIBILITYDELTA_H

#include <vector>

#include <MMORPGServer/Server/Manager/ChunkCoordinate.h>

namespace Server {

class WorldVisibilityDelta {
public:
    std::vector<int> charactersEntered;
    std::vector<int> charactersLeft;
    std::vector<int> viewersLeft;
    std::vector<int> creaturesEntered;
    std::vector<int> creaturesLeft;
    std::vector<int> groundItemsEntered;
    std::vector<int> groundItemsLeft;
    std::vector<ChunkCoordinate> terrainChunksEntered;
    std::vector<ChunkCoordinate> terrainChunksLeft;
};

} // namespace Server

#endif // WORLDVISIBILITYDELTA_H
