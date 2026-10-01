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
#include <MMORPGServer/Server/Runtime/Creature/CreatureRuntime.h>

namespace Server {

class WorldSpatialIndex {
public:
    WorldSpatialIndex( const std::map<int, std::unique_ptr<CharacterRuntime>>& characters, const std::map<int, std::unique_ptr<CreatureRuntime>>& creatures );

    void addCharacter( Engine::CharacterModel* character );
    void removeCharacter( Engine::CharacterModel* character );
    void updateCharacterChunk( Engine::CharacterModel* character, const Engine::EntityPositionModel& previousPosition );

    ChunkCoordinate chunkCoordinateFor( const Engine::EntityPositionModel& position ) const;

    std::vector<int> charactersNear( int idCharacter ) const;
    std::vector<int> charactersNear( const Engine::EntityPositionModel& position ) const;
    bool hasCharacterNear( const Engine::EntityPositionModel& position ) const;
    Engine::CharacterModel* characterAt( int x, int y, int z ) const;
    Engine::CharacterModel* nearestCharacterWithin( const Engine::EntityPositionModel& position, int radius ) const;

    std::vector<Engine::CreatureModel> creaturesNear( const Engine::EntityPositionModel& position ) const;
    Engine::CreatureModel* creatureAt( int x, int y, int z ) const;

    bool isPositionOccupied( const Engine::EntityPositionModel& position ) const;

private:
    std::unordered_map<ChunkCoordinate, std::vector<Engine::CharacterModel*>, ChunkCoordinateHash> _charactersByChunk;
    const std::map<int, std::unique_ptr<CharacterRuntime>>& _characters;
    const std::map<int, std::unique_ptr<CreatureRuntime>>& _creatures;
};

} // namespace Server

#endif // WORLDSPATIALINDEX_H
