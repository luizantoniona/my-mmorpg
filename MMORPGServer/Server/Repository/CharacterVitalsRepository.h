#ifndef CHARACTERVITALSREPOSITORY_H
#define CHARACTERVITALSREPOSITORY_H

#include <memory>

#include <MMORPGEngine/Entity/Character/CharacterVitalsModel.h>
#include <MMORPGEngine/Entity/EntityVitalsModel.h>
#include <MMORPGServer/Server/Repository/Repository.h>

namespace Server {

class CharacterVitalsRepository : public Repository {
public:
    explicit CharacterVitalsRepository();

    bool create( int idCharacter, const Engine::EntityVitalsModel& vitals );

    std::unique_ptr<Engine::CharacterVitalsModel> find( int idCharacter );

    bool save( int idCharacter, const Engine::EntityVitalsModel& vitals );
};

} // namespace Server

#endif // CHARACTERVITALSREPOSITORY_H
