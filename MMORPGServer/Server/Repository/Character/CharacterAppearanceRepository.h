#ifndef CHARACTERAPPEARANCEREPOSITORY_H
#define CHARACTERAPPEARANCEREPOSITORY_H

#include <MMORPGEngine/Entity/Character/CharacterAppearanceModel.h>
#include <MMORPGServer/Server/Repository/Repository.h>

namespace Server {

class CharacterAppearanceRepository : public Repository {
public:
    explicit CharacterAppearanceRepository();

    bool create( int idCharacter, const Engine::CharacterAppearanceModel& appearance );

    Engine::CharacterAppearanceModel find( int idCharacter );
};

} // namespace Server

#endif // CHARACTERAPPEARANCEREPOSITORY_H
