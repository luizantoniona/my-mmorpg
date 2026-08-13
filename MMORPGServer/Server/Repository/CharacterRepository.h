#ifndef CHARACTERREPOSITORY_H
#define CHARACTERREPOSITORY_H

#include <memory>

#include <MMORPGEngine/Entity/Character/CharacterModel.h>
#include <MMORPGServer/Server/Repository/Repository.h>

namespace Server {

class CharacterRepository : public Repository {
public:
    explicit CharacterRepository();

    int createCharacter( const int idAccount, const std::string& dsName );
    bool deleteCharacter( int idCharacter );
    bool updateCharacter( Engine::CharacterModel character );

    std::unique_ptr<Engine::CharacterModel> findByIdAccountAndIdCharacter( const int idAccount, const int idCharacter );
};

} // namespace Server

#endif // CHARACTERREPOSITORY_H
