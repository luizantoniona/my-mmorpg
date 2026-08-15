#ifndef ACCOUNTREPOSITORY_H
#define ACCOUNTREPOSITORY_H

#include <memory>
#include <string>

#include <MMORPGEngine/Account/AccountModel.h>
#include <MMORPGServer/Server/Repository/Repository.h>

namespace Server {

class AccountRepository : public Repository {
public:
    AccountRepository();

    bool createAccount( std::string& username, std::string& password );

    std::unique_ptr<Engine::AccountModel> findByUsername( const std::string& username );
    std::unique_ptr<Engine::AccountModel> findByUsernameAndPassword( const std::string& username, const std::string& password );

    std::vector<Engine::AccountCharacterModel> findCharacters( const int idAccount );
};

} // namespace Server

#endif // ACCOUNTREPOSITORY_H
