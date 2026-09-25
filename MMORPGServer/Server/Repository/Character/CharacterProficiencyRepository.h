#ifndef CHARACTERPROFICIENCYREPOSITORY_H
#define CHARACTERPROFICIENCYREPOSITORY_H

#include <memory>
#include <vector>

#include <MMORPGEngine/Entity/Character/CharacterProficiencyModel.h>
#include <MMORPGServer/Server/Repository/Repository.h>

namespace Server {

class CharacterProficiencyRepository : public Repository {
public:
    explicit CharacterProficiencyRepository();

    bool create( int idCharacter, const Engine::CharacterProficiencyModel& proficiency );

    std::unique_ptr<Engine::CharacterProficiencyModel> find( int idCharacter, uint32_t idItemType );
    std::vector<Engine::CharacterProficiencyModel> findAll( int idCharacter );

    bool save( int idCharacter, const Engine::CharacterProficiencyModel& proficiency );
};

} // namespace Server

#endif // CHARACTERPROFICIENCYREPOSITORY_H
