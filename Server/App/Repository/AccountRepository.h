#ifndef ACCOUNTREPOSITORY_H
#define ACCOUNTREPOSITORY_H

#include <memory>
#include <string>

#include <Engine/Account/AccountModel.h>
#include <Repository/Repository.h>

namespace Server {

class AccountRepository : public Repository {
public:
    AccountRepository();

    bool createAccount( std::string& username, std::string& password );

    std::unique_ptr<Engine::AccountModel> findByUsername( const std::string& username );
    std::unique_ptr<Engine::AccountModel> findByUsernameAndPassword( const std::string& username, const std::string& password );
};

} // namespace Server

#endif // ACCOUNTREPOSITORY_H
