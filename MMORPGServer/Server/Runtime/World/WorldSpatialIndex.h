#ifndef WORLDSPATIALINDEX_H
#define WORLDSPATIALINDEX_H

#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/Creature/CreatureModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGServer/Server/Manager/ChunkCoordinate.h>
#include <MMORPGServer/Server/Runtime/Character/CharacterRuntime.h>
#include <MMORPGServer/Server/Runtime/Chunk/ChunkRuntime.h>
#include <MMORPGServer/Server/Runtime/Creature/CreatureRuntime.h>
#include <MMORPGServer/Server/Runtime/World/Spawn/WorldSpawnAreaRuntime.h>

namespace Server {

class WorldSpatialIndex {
public:
    WorldSpatialIndex( const std::map<int, std::unique_ptr<CharacterRuntime>>& characters, const std::map<int, std::unique_ptr<CreatureRuntime>>& creatures );

    void addCharacter( Engine::CharacterModel* character );
    void removeCharacter( Engine::CharacterModel* character );
    void updateCharacterChunk( Engine::CharacterModel* character, const Engine::EntityPositionModel& previousPosition );

    void addCreature( CreatureRuntime* creature );
    void removeCreature( CreatureRuntime* creature );
    void updateCreatureChunk( CreatureRuntime* creature, const Engine::EntityPositionModel& previousPosition );

    void addGroundItem( int idGroundItem, const Engine::EntityPositionModel& position );
    void removeGroundItem( int idGroundItem, const Engine::EntityPositionModel& position );

    void addSpawnArea( WorldSpawnAreaRuntime* spawnArea );

    ChunkCoordinate chunkCoordinateFor( const Engine::EntityPositionModel& position ) const;

    const ChunkRuntime* findChunk( const ChunkCoordinate& coordinate ) const;
    std::vector<ChunkRuntime*> activeChunks() const;

    std::vector<int> charactersVisibleFrom( int idCharacter ) const;
    std::vector<int> charactersVisibleFrom( const Engine::EntityPositionModel& position ) const;
    std::vector<int> charactersViewing( int idCharacter ) const;
    std::vector<int> charactersViewing( const Engine::EntityPositionModel& position ) const;
    Engine::CharacterModel* characterAt( int x, int y, int z ) const;
    Engine::CharacterModel* nearestCharacterWithin( const Engine::EntityPositionModel& position, int radius ) const;

    std::vector<Engine::CreatureModel> creaturesVisibleFrom( const Engine::EntityPositionModel& position ) const;
    Engine::CreatureModel* creatureAt( int x, int y, int z ) const;

    std::vector<int> groundItemsVisibleFrom( const Engine::EntityPositionModel& position ) const;

    bool isPositionOccupied( const Engine::EntityPositionModel& position ) const;
    bool isAreaOccupied( int x, int y, int z, int width, int height, int ignoredIdCreature = -1 ) const;

private:
    ChunkRuntime& chunkOrCreate( const ChunkCoordinate& coordinate );
    std::vector<const ChunkRuntime*> chunksAround( const Engine::EntityPositionModel& position, int floorOffset ) const;
    void observe( const ChunkCoordinate& center, bool observed );

private:
    std::unordered_map<ChunkCoordinate, std::unique_ptr<ChunkRuntime>, ChunkCoordinateHash> _chunks;
    const std::map<int, std::unique_ptr<CharacterRuntime>>& _characters;
    const std::map<int, std::unique_ptr<CreatureRuntime>>& _creatures;
};

} // namespace Server

#endif // WORLDSPATIALINDEX_H
