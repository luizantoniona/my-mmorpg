#ifndef ACCOUNTREPOSITORY_H
#define ACCOUNTREPOSITORY_H

#include <memory>
#include <string>

#include <Engine/Database/Repository.h>

#include "AccountModel.h"

namespace Engine {

class AccountRepository : public Repository {
public:
    AccountRepository();

    bool createAccount( std::string& username, std::string& password );

    std::unique_ptr<AccountModel> findByUsername( const std::string& username );
    std::unique_ptr<AccountModel> findByUsernameAndPassword( const std::string& username, const std::string& password );
};

} // namespace Engine

#endif // ACCOUNTREPOSITORY_H
