#ifndef CHARACTERREPOSITORY_H
#define CHARACTERREPOSITORY_H

#include <memory>
#include <vector>

#include <MMORPGEngine/Character/CharacterModel.h>
#include <MMORPGServer/Server/Repository/Repository.h>

namespace Server {

class CharacterRepository : public Repository {
public:
    explicit CharacterRepository();

    int createCharacter( const int idAccount, const std::string& dsName );
    bool deleteCharacter( int idCharacter );
    bool updateCharacter( Engine::CharacterModel character );

    std::vector<std::unique_ptr<Engine::CharacterModel> > findAllByIdAccount( const int idAccount );

    std::unique_ptr<Engine::CharacterModel> findByIdAccountAndIdCharacter( const int idAccount, const int idCharacter );
};

} // namespace Server

#endif // CHARACTERREPOSITORY_H
