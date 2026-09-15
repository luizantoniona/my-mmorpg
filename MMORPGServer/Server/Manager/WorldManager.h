#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/World/WorldModel.h>

namespace Server {

class WorldManager {
public:
    WorldManager();
    ~WorldManager();

    void initialize( const std::string& worldPath );
    void finalize();

    Engine::WorldModel* world();
    const Engine::WorldModel* world() const;

    Engine::CharacterModel* addCharacter( std::unique_ptr<Engine::CharacterModel> character );
    void removeCharacter( int idCharacter );
    Engine::CharacterModel* character( int idCharacter );
    std::map<int, Engine::EntityPositionModel> characterPositions();

private:
    std::atomic<bool> _running;
    std::thread _thread;
    std::unique_ptr<Engine::WorldModel> _world;
    std::mutex _mutex;
    std::map<int, std::unique_ptr<Engine::CharacterModel>> _characters;
};

} // namespace Server

#endif // WORLDMANAGER_H
