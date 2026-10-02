#ifndef CHARACTERINVENTORYREPOSITORY_H
#define CHARACTERINVENTORYREPOSITORY_H

#include <memory>
#include <vector>

#include <MMORPGEngine/Entity/Character/CharacterInventoryModel.h>
#include <MMORPGServer/Server/Repository/Repository.h>

namespace Server {

class CharacterInventoryRepository : public Repository {
public:
    explicit CharacterInventoryRepository();

    bool create( int idCharacter, const Engine::CharacterInventoryModel& inventory );

    std::unique_ptr<Engine::CharacterInventoryModel> find( int idCharacter, uint32_t position );
    std::vector<Engine::CharacterInventoryModel> findAll( int idCharacter );

    bool save( int idCharacter, const Engine::CharacterInventoryModel& inventory );
};

} // namespace Server

#endif // CHARACTERINVENTORYREPOSITORY_H
