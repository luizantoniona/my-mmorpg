#ifndef CHARACTERPOSITIONREPOSITORY_H
#define CHARACTERPOSITIONREPOSITORY_H

#include <memory>

#include <MMORPGEngine/Entity/Character/CharacterPositionModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGServer/Server/Repository/Repository.h>

namespace Server {

class CharacterPositionRepository : public Repository {
public:
    explicit CharacterPositionRepository();

    bool create( int idCharacter, const Engine::EntityPositionModel& position );

    std::unique_ptr<Engine::CharacterPositionModel> find( int idCharacter );

    bool save( int idCharacter, const Engine::EntityPositionModel& position );
};

} // namespace Server

#endif // CHARACTERPOSITIONREPOSITORY_H
