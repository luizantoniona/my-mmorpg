#ifndef CHARACTERPOSITIONREPOSITORY_H
#define CHARACTERPOSITIONREPOSITORY_H

#include <memory>

#include <MMORPGEngine/Entity/Character/CharacterPositionModel.h>
#include <MMORPGServer/Server/Repository/Repository.h>

namespace Server {

class CharacterPositionRepository : public Repository {
public:
    explicit CharacterPositionRepository();

    std::unique_ptr<Engine::CharacterPositionModel> find( int idCharacter );
    bool save( const Engine::CharacterPositionModel& position );
};

} // namespace Server

#endif // CHARACTERPOSITIONREPOSITORY_H
