#ifndef CHARACTERREPOSITORY_H
#define CHARACTERREPOSITORY_H

#include <memory>
#include <vector>

#include <Engine/Database/Repository.h>

#include "CharacterModel.h"

namespace Engine {

class CharacterRepository : public Repository {
public:
    explicit CharacterRepository();

    int createCharacter( const int idAccount, const std::string& dsName );
    bool deleteCharacter( int idCharacter );
    bool updateCharacter( CharacterModel character );

    std::vector<std::unique_ptr<CharacterModel>> findAllByIdAccount( const int idAccount );

    std::unique_ptr<CharacterModel> findByIdAccountAndIdCharacter( const int idAccount, const int idCharacter );
};

} // namespace Engine

#endif // CHARACTERREPOSITORY_H
